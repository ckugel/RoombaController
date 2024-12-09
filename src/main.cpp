#include "../include/imgui/imgui.h"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Pose2D.hpp"
#include "Node.hpp"
#include "Pillar.hpp"
#include "Hole.hpp"
#include "Field.hpp"
#include <vector>
#include <netinet/in.h>
#include <chrono>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <sstream>

#define BOT_CONNECT 0
#if BOT_CONNECT
    #define ADDRESS "192.168.1.1"
    #define PORT 288
#else
    #define ADDRESS "127.0.0.1"
    #define PORT  65432
#endif

enum MovementType {
    MOVE_FORWARD,
    MOVE_BACKWARDS,
    MOVE_FORWARD_SMART,
    TURN_TO_ANGLE,
    SCAN
};

std::queue<std::string> sendQueue;
std::mutex queueMutex;
std::condition_variable sendCondition;

struct Move {
    double quantity;
    MovementType type;
};

std::atomic<bool> stopClient(false);


void addToQueue(const std::string& message) {
    // std::cout << message << std::endl;
    queueMutex.lock();
    sendQueue.push(message);
    queueMutex.unlock();
}

void sendAngleToQueue(int16_t angle) {
    char buff[8];
    sprintf(buff, "t%03d", angle);


    // std::cout << std::string(buff) << std::endl;
    sendQueue.emplace(buff);
}

void sendDistanceToQueue(uint16_t angle) {
    char buff[8];
    sprintf(buff, "r%03d", angle);
    sendQueue.emplace(buff);
}

void pathToRoutine(std::vector<Pose2D> path, std::vector<Move>& routine) {
    // start at 1 so that we can gurantee that we can look back.
    for (uint8_t move = 1; move < ((path.size() - 1)*2); move += 2) {
        // for every point we want to point and move
        Pose2D pointOld = path[move / 2];
        Pose2D pointNew = path[(move / 2) + 1];
        // angle to turn to
        double angle = pointNew.angleTo(pointOld) + pointNew.getHeading();
        // magnitude
        double magnitude = pointNew.distanceTo(pointOld);
        routine.push_back((Move) {.quantity = angle, .type = TURN_TO_ANGLE});
        routine.push_back((Move) {.quantity = magnitude, .type = MOVE_FORWARD_SMART});
    }
}

std::string parsePathIntoRoutine(const std::vector<Pose2D>& path) {
    std::stringstream toSend;
    std::vector<Move> routine;
    // std::cout << "routine length: " << routine.size() << std::endl;
    pathToRoutine(path, routine);

    toSend << "R";

    for (uint8_t i = 0; i < routine.size(); i++) {
        static char buffer[50];
        // sprintf(buffer, "p %0.3f %0.3f " , path[i].getX(), path[i].getY());
        // // routine[i] is all 0
        sprintf(buffer, "p %0.3f %d p", routine[i].quantity, routine[i].type);
        toSend << std::string(buffer);
    }

    toSend << "R";

    return toSend.str();
}

void readAndLog(int socket, std::mutex& fieldMutex, Field& field) {
	const uint16_t BUFF_SIZE = 1024;

	static char name_buff[50];
  time_t now = time(0);
  strftime(name_buff, sizeof(name_buff), "log/%Y%m%d_%H%M%S.log", localtime(&now));
  std::string str_name(name_buff);

  std::ofstream logFile(str_name);

  while (!(stopClient.load())) {

    char buff[BUFF_SIZE];

    // expect a "Handshake" response to be echoed
    size_t bytesRead = read(socket, buff, BUFF_SIZE);

    std::string response(buff, bytesRead);

    if (bytesRead > 0) {
        logFile << response;
    }

    std::istringstream stream(response);
    char tag;
    while (stream >> tag) {
	switch(tag) {
	    case 'F':
            // send the routine
            {
                // generate path and return it
                fieldMutex.lock();

                field.discretizeGraph();
                field.weightGraph();
                std::vector<Pose2D> path = field.makePath();
                Pose2D oldCenter = field.getDesiredDestination();
                if (path.empty()) {
                    // try a new desired position until its not empty
                    for (uint8_t i = 0; i < 10; i += 5) {
                        for (double j = 0; j < 2 * M_PI; j += M_PI * 1/4) {
                            Pose2D newDesired = Pose2D::fromPolar(i, j);
                            newDesired.plus(oldCenter);
                            if (!Field::outOfBounds(newDesired)) {
                                field.updateDesired(newDesired);
                                field.discretizeGraph();
                                field.weightGraph();
                                path = field.makePath();
                                if (!path.empty()) {
                                    sendQueue.push(parsePathIntoRoutine(path));
                                    break;
                                }
                            }
                        }
                    }
                }

                fieldMutex.unlock();
            }
		    break;
	    case 'o':
			{
			fieldMutex.lock();
			Pillar toAdd = Pillar::parseFromStream(stream);
			// std::cout << "x: " << toAdd.getX() << " y: " << toAdd.getY() << " radius: " << toAdd.getRadius() << std::endl;
			toAdd.getPose().plus(field.getBotPose().getPose());
			field.addPillar(toAdd);
			fieldMutex.unlock();
			}
			break;
	    case 'd':
			{
				Pose2D other = Pose2D::parseFromStream(stream);
                field.updateDesired(other);
			}
			break;
		case 'E':
		    {
			double m;
			char cardinality;
			if (stream >> m >> cardinality) {
			    field.addEdgeMeasurement(m, (Cardinality) cardinality);
			}
			// first term is the raw X or Y value next value is the direction
		    }
		    break;
		case 'h':
		    {
			fieldMutex.lock();
			// data is coming in the format " X Y Theta "
			Pose2D holeMeasurment = Pose2D::parseFromStream(stream);
			field.getManager().addPoint(holeMeasurment);
			fieldMutex.unlock();
		    }
		    break;
		case 'H':
		    {
			// data is coming in the format " X1 Y1 X2 Y2 "
			fieldMutex.lock();
			double x1, y1, x2, y2;
			if (stream >> x1 >> y1 >> x2 >> y2) {
			    Hole hole(x1, y1, x2, y2);
			    field.getManager().addHole(hole);
			}
			fieldMutex.unlock();
		    }
		    break;
		case 'b':
		    {
			fieldMutex.lock();
			field.updateBotPose(Pose2D::parseFromStream(stream));
			fieldMutex.unlock();
		    }
			break;
	    default:
			break;
	}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // no matter what we are going to log this in a file, however we should also update certain fields

  }

  logFile.close();

}

// connect to Roomba server
void connectTCP(Field& field, std::mutex& fieldMutex) {
 int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serverAddress{
	.sin_family = AF_INET,
  	.sin_port = htons(PORT),
  	.sin_addr = inet_addr(ADDRESS)
  };

  // https://beej.us/guide/bgnet/html/#blocking
  // fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    bool connected = false;
    while (!connected) {
	try {
	    if(connect(clientSocket, (struct sockaddr*)& serverAddress, sizeof(serverAddress))) {
		    throw std::exception();
	    }
	    connected = true;
	}
	catch (std::exception& e) {
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    connected = false;
	}

    }

  // spawn read and log thread here
  std::thread readThread(readAndLog, std::ref(clientSocket), std::ref(fieldMutex), std::ref(field));

  while(!stopClient.load()) {
	if (!sendQueue.empty()) {
	    std::string message = sendQueue.front();
	    if (message.compare("q") == 0) {
		stopClient.store(true);
	    }
	    sendQueue.pop();
	    send(clientSocket, message.c_str(), message.length(), 0);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }


  readThread.join();
  close(clientSocket);
}

// Simple function to set up OpenGL and ImGui context
void setupImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void DrawCircle(ImDrawList* drawList, const ImVec2& center, float radius, ImU32 color) {
  drawList->AddCircle(center, radius, color, 0, 0.2f);
}

Pose2D ScreenToCoords(ImVec2 coords, ImVec2 offset, ImVec2 scaling) {
	double x = (coords.x - offset.x) / scaling.x;
	double y = MAX_Y - (coords.y - offset.y) / scaling.y;
	return {x, y};
}

ImVec2 coordsToScreen(ImVec2 offset, ImVec2 scaling, double x, double y) {
    double xN = offset.x + (scaling.x * x);
    double yN = offset.y + (scaling.y * (MAX_Y - y));
    return ImVec2(xN, yN);
}

ImVec2 coordsToScreen(ImVec2 offset, ImVec2 scaling, const Pose2D& position) {
    return coordsToScreen(offset, scaling, position.getX(), position.getY());
}

void ShowPillarOnWindow(ImDrawList* drawList, Pillar pillar, ImU32 color, ImVec2 offset, ImVec2 scaling) {
	ImVec2 center = coordsToScreen(offset, scaling, pillar.getPose());
	float radius = pillar.getRadius() * 2.5;
	// std::cout << "haven't drawn yet" << std::endl;
	DrawCircle(drawList, center, radius, color);
}

/**
 * Draws a triangle on the bot position facing in the direction that the bot is.
 * The color of the triangle will be lime green.
 *
 * @param drawList the draw list for the window
 * @param botPose the position of the bot
 * @param offset the offset of the screen for cartesian coordinates
 */
void drawBotPose(ImDrawList* drawList, const Pose2D& botPose, ImVec2 offset, ImVec2 scaling) {
    ImU32 color = IM_COL32(144, 238, 144, 200);
    
    // calculate the position of the first point
    // should be botPose + (botRadius * 1.5 @ bot heading)
	double scaleAmount = 2;
    Pose2D lineOutOfCenter = Pose2D::fromPolar(BOT_RADIUS * 1.5 * scaleAmount, 0);
    lineOutOfCenter.transformPose(botPose);
    const ImVec2 p1 = coordsToScreen(offset, scaling, lineOutOfCenter);
    Pose2D sideLine = Pose2D::fromPolar(scaleAmount * BOT_RADIUS * 0.75, Pose2D::degreesToRadians(120));
    sideLine.transformPose(botPose);
    const ImVec2 p2 = coordsToScreen(offset, scaling, sideLine);
    Pose2D otherSide = Pose2D::fromPolar(BOT_RADIUS * scaleAmount * 0.75, Pose2D::degreesToRadians(240));
    otherSide.transformPose(botPose);
    const ImVec2 p3 = coordsToScreen(offset, scaling, otherSide);
    
    drawList->AddTriangle(p1, p2, p3, color);
}

void drawRectangle(ImDrawList* drawList, ImVec2 offset, ImVec2 scaling, const Pose2D& p1, const Pose2D& p2) {
    // const Pose2D MinPosition = Pose2D(std::min(p1.getX(), p2.getX()), std::min(p1.getY(), p2.getY()));
    // const Pose2D MaxPosition = Pose2D(std::max(p1.getX(), p2.getX()), std::max(p1.getY(), p2.getY()));
    // fun math time
    const double xCenter = (p1.getX() + p2.getX()) / 2;
    const double yCenter = (p1.getY() + p2.getY()) / 2;
    const double xDiagonal = (p1.getX() - p2.getX()) / 2;
    const double yDiagonal = (p1.getY() - p2.getY()) / 2;

    const Pose2D p3(xCenter - yDiagonal, yCenter + xDiagonal);
    const Pose2D p4(xCenter + yDiagonal, yCenter - xDiagonal);

    const ImVec2 m1 = coordsToScreen(offset, scaling, p1);
    const ImVec2 m2 = coordsToScreen(offset, scaling, p2);
    const ImVec2 m3 = coordsToScreen(offset, scaling, p3);
    const ImVec2 m4 = coordsToScreen(offset, scaling, p4);
    //     std::    cout << "p2 position: " << p2.getY() << std::endl;

    drawList->AddQuadFilled(m1, m3, m2, m4, IM_COL32(255, 165, 0, 170));
}

void ShowFieldWindow(std::mutex* pillarsMutex, std::vector<Pose2D>& path, Field& field, std::atomic<bool>& showNodes, std::atomic<bool>& showEdges) {
	ImGui::Begin("Field");
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowSize(MAX_X * 2.5, MAX_Y * 2.5);
	ImGui::SetWindowSize(windowSize);
    ImVec2 windowPos = ImGui::GetWindowPos();
	windowPos.x += windowSize.x / 100;
	windowPos.y += windowSize.y / 100;
    ImVec2 offset =  ImVec2(windowPos.x + windowSize.x / 50, windowPos.y - windowSize.y / 50);
    ImVec2 scalingFactor = ImVec2(windowSize.x / MAX_X, windowSize.y / MAX_Y);
    
    pillarsMutex->lock();

    std::vector<Pillar> pillars = field.getCopyPillars();

    for (const Pillar& pillar: pillars) {
		ShowPillarOnWindow(drawList, pillar, IM_COL32(255, 0, 0, 200), offset, scalingFactor);
    }

    std::vector<Hole> holes = field.getManager().getHoles();
//     std::cout << "size: " << holes.size() << std::endl; // output 1
    for (Hole hole: holes) {
		// std::cout << "X1 Y1 X2 Y2: {" << hole.getOneSquareCorner().getX() << " " << hole.getOneSquareCorner().getY() << " " << hole.getSecondSquareCorner().getX() << " " << hole.getSecondSquareCorner().getY() << std::endl;
		drawRectangle(drawList, offset, scalingFactor, hole.getOneSquareCorner(), hole.getSecondSquareCorner());
    }
    
    drawBotPose(drawList, field.getBotPose().getPose(), offset, scalingFactor);

    if (showNodes.load()) {
        std::vector<Node<Pose2D> *> nodes = field.getGraph().getNodes();

        for (Node<Pose2D> *&node: nodes) {
            Pose2D position = node->getData();

            ImVec2 center = coordsToScreen(offset, scalingFactor, position.getX(), position.getY());
            float radius = BOT_RADIUS * 2.5;
            DrawCircle(drawList, center, radius, IM_COL32(120, 120, 0, 200));
            // draw a line from every node to the adjacent yes we will double count draws with this
            //  std::vector<Node<Pose2D>*> adjacent = getAdj(nodes[nodeIndex]);

        }
    }

    if (showEdges.load()) {
        std::vector<Node<Pose2D> *> nodes = field.getGraph().getNodes();
        for (Node<Pose2D> *&node: nodes) {
            std::vector<Node<Pose2D> *> adjacent = field.getGraph().getAdj(node);
            for (Node<Pose2D> *&adj: adjacent) {
                ImVec2 p1 = coordsToScreen(offset, scalingFactor, node->getData());
                ImVec2 p2 = coordsToScreen(offset, scalingFactor, adj->getData());
                drawList->AddLine(p1, p2, IM_COL32(100, 100, 100, 100), 2);
            }
        }
    }

    
    for (uint8_t i = 0; i < path.size(); i++) {
		if (i > 0) {
			ImVec2 p1 = coordsToScreen(offset, scalingFactor, path[i - 1].getX(), path[i - 1].getY());
			ImVec2 p2 = coordsToScreen(offset, scalingFactor, path[i].getX(), path[i].getY());
			drawList->AddLine(p1, p2, IM_COL32(100, 100, 100, 100), 2);
		}

		ImVec2 center = coordsToScreen(offset, scalingFactor, path[i].getX(), path[i].getY());
		float radius = BOT_RADIUS * 2.5;
		DrawCircle(drawList, center, radius, IM_COL32(30, 120, 220, 150));
	}

    pillarsMutex->unlock();

	ImVec2 mousePos = ImGui::GetMousePos();
// 	std::string mousePosStr = std::to_string((int)mousePos.x) + ", " + std::to_string((int)mousePos.y);
	ImGui::Text("mouse: %d, %d", (int)mousePos.x, (int)mousePos.y);
	Pose2D transformed = ScreenToCoords(ImVec2((int) mousePos.x, (int) mousePos.y), offset, scalingFactor);
	ImGui::Text("Pose on field: %d, %d", (int) transformed.getX(), (int) transformed.getY());

    ImGui::End();
}



int main() {
  if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1880, 900, "Roomba Dashboard", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    setupImGui(window);
    Pose2D desired(0, 0);
    
    // used to hold GUI slider values
    float angleSend = 0;
    int driveForward = 0;

    std::atomic<bool> showNodes;
    showNodes.store(false);
    std::atomic<bool> showEdges;
    showEdges.store(false);

    std::mutex pillarsMutex;
    std::vector<Pose2D> path;

    Field field;
    // Pose2D toAdd(0, 0, 0);
    // graph.addNode(new Node<Pose2D>(toAdd));
    // connectTCP(pillars, pillarsMutex, desired);
    std::thread tcpConnect(connectTCP, std::ref(field), std::ref(pillarsMutex));
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // bool open;
        // ImGui::ShowDemoWindow(&open);
        ShowFieldWindow(&pillarsMutex, path, field, showNodes, showEdges);
        // std::cout << "About to begin" << std::endl;
	
        // Your ImGui code here
        ImGui::Begin("Control Panel");
        // 	std::cout << "began" << std::endl;
        //ImGui::Text("This is some text");
        {
            if (ImGui::Button("Forward")) {
                addToQueue("w");
            }

            if (ImGui::Button("Backward")) {
                addToQueue("s");
            }

            if (ImGui::Button("Counter Clockwise")) {
                addToQueue("a");
            }

            if (ImGui::Button("Clockwise")) {
                addToQueue("d");
            }

            if (ImGui::Button("Stop")) {
                addToQueue(" ");
            }

            if (ImGui::Button("Scan")) {
                addToQueue("g");
            }

            if (ImGui::Button("Auton")) {
                addToQueue("h");
            }

            if (ImGui::Button("Quit all")) {
                addToQueue("q");
            }

            if (ImGui::RadioButton("Show nodes", false)) {
                showNodes.store(!showNodes.load());
            }

            if (ImGui::RadioButton("Show edges", false)) {
                showEdges.store(!showEdges.load());
            }
        }

        if (ImGui::Button("Generate path")) {
            pillarsMutex.lock();
            /*
                std::cout << "desire: " << desired << std::endl;
                for (int i = 0; i < graph->getNodes().size(); i++) {
                std::cout << "node: " << i << ".  " << graph->getNodes()[i]->getData().getX() << std::endl;
            }

            std::cout << "Attempting to make a path between: " << graph->getNodes().front()->getData().getX() << ", " << graph->getNodes().front()->getData().getY() << std::endl;
            std::cout << "To: " <<graph->getNodes().back()->getData().getX() << ", " << graph->getNodes().back()->getData().getY() << std::endl;
            // Attempting to make a path between: 0, 0
            // To: -100, -100
            */
            path =  field.makePath();
            //  std::cout << "PATH NODE SIZE: " << pathNodes.size() << std::endl;
            // path.push_back(botPose.getPose());
	}

	if (ImGui::Button("send planned path")) {
	    // string message = parsePathIntoRoutine(path);
	    addToQueue(parsePathIntoRoutine(path));
	}

	ImGui::SliderAngle("Turn angle", &angleSend);


	if (ImGui::Button("Send turn")) {
	  sendAngleToQueue((int16_t) (angleSend * 180 / M_PI));
	}

	ImGui::SliderInt("Drive x cm", &driveForward, 0, 999);

	if (ImGui::Button("Drive x")) {
	    sendDistanceToQueue(driveForward);
	}

	if (ImGui::Button("Discretize")) {
	    // std::cout << "HUH: " << pillars[0].getX() << std::endl;
	   field.discretizeGraph();
	}

	if (ImGui::Button("Weight")) {
	    field.weightGraph();
	}
	// std::cout << "About to end" << std::endl;

        ImGui::End();
        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    stopClient.store(true);

    tcpConnect.join();
  
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
