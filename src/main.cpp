#include "../include/imgui/imgui.h"
// #include "../include/imgui/imgui_demo.cpp"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Pose2D.hpp"
#include "Node.hpp"
#include "Graph.hpp"
#include "Pillar.hpp"
#include <vector>
#include <netinet/in.h>
#include <chrono>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <sstream>
#include <iomanip>

#define BOT_CONNECT 0
#if BOT_CONNECT
    #define ADDRESS "192.168.1.1"
    #define PORT 288
#else
    #define ADDRESS "127.0.0.1"
    #define PORT  65432
#endif


#define SCREEN_SCALE 3.0f
#define BOT_RADIUS 6

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

/*
std::unique_ptr<Graph<Pose2D>> discretizeField(std::vector<Pillar> fields, Pose2D botPose, uint8_t desiredIndex) {

  std::unique_ptr<Graph<Pose2D>> graph = std::make_unique<Graph<Pose2D>>();  
  
  


  return graph;
}
*/


void readAndLog(int socket, std::vector<Pillar>& field, std::mutex& fieldMutex, uint8_t& desired, Pillar& botPose) {
  const uint16_t BUFF_SIZE = 1024;

  /*
  auto now = std::chrono::system_clock::now(); 
  std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

  std::tm* timeInfo = std::localtime(&timeNow);
  
  std::stringstream filename;
  filename << "log_" << std::put_time(timeInfo, "%Y%m%d_%H%M%S") << ".txt";
*/

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
		    field.clear();
		break;
	    case 'd':
		{
			uint8_t readAble;
			if (stream >> readAble) {
			    desired = readAble - '0';
			}
			else {
			    logFile << "Could not parse stream for: d" << std::endl;
			}
		}
		break;
	    case 'o':
		    {	fieldMutex.lock();
			Pillar toAdd = Pillar::parseFromStream(stream);
			// std::cout << "x: " << toAdd.getX() << " y: " << toAdd.getY() << " radius: " << toAdd.getRadius() << std::endl; 
			field.push_back(toAdd);
			fieldMutex.unlock();
		    }
		break;
		case 'b':
		    {
			fieldMutex.lock();
			botPose = Pillar::parseFromStream(stream);
			fieldMutex.unlock();
		    }
		break;
	    default:
		break;
	}
    }

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
/*
    if (bytesRead > 0) {
      std::string response(buff, bytesRead);
      logFile << response << std::endl;
      if (response.find("quit") != std::string::npos) {
	std::cout << "exiting tcp server" << std::endl;
	stopClient.store(true);
      }
      else if (response.) {

	    }
    }
    else {
      // sleep and wait
      // There should be an os signal to handle this
      // potentially fcntl
    }
*/
    // no matter what we are going to log this in a file, however we should also update certain fields

  }

  logFile.close();

}

// connect to Roomba server
void connectTCP(std::vector<Pillar>& field, std::mutex& fieldMutex, uint8_t& desired, Pillar& botPose) {
 int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = inet_addr(ADDRESS);

  // https://beej.us/guide/bgnet/html/#blocking
  // fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    bool connected = false;
    while (!connected) {
	try {
	    int status = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	    connected = true;
	}
	catch (std::exception e) {
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    connected = false;
	}

    }

  // std::cout << "CONNECT" << std::endl;

  // maybe make thread here

  const char* message = "Handshake";
  send(clientSocket, message, strlen(message), 0);

    // readAndLog(clientSocket, field, fieldMutex, desired);

  // spawn read and log thread here
  std::thread readThread(readAndLog, std::ref(clientSocket), std::ref(field), std::ref(fieldMutex), std::ref(desired), std::ref(botPose));

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
;;;}

void ShowPillarOnWindow(ImDrawList* drawList, Pillar pillar, ImU32 color, ImVec2 offset) {
    	ImVec2 center = ImVec2(offset.x + pillar.getX() * SCREEN_SCALE, offset.y - pillar.getY() * SCREEN_SCALE);
	float radius = pillar.getRadius() * SCREEN_SCALE;
	// std::cout << "haven't drawn yet" << std::endl;
	DrawCircle(drawList, center, radius, color);
}

ImVec2 coordsToScreen(ImVec2 offset, double x, double y) {
    double xN = ((x * SCREEN_SCALE) + offset.x);
    double yN = ((-y * SCREEN_SCALE) + offset.y);
    return ImVec2(xN, yN);
}

void ShowFieldWindow(std::vector<Pillar> pillars, std::mutex* pillarsMutex, Pillar botPose, Graph<Pose2D>* graph, std::vector<Pose2D>& path) {
  ImGui::Begin("Field");
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 offset = ImVec2(windowPos.x + 50 + (windowSize.x / 2), windowPos.y + 50 + (windowSize.y / 2));
    
    pillarsMutex->lock();
    // std::cout << pillars.size() << std::endl;

    for (Pillar pillar: pillars) {
	ShowPillarOnWindow(drawList, pillar, IM_COL32(255, 0, 0, 120), offset);
    }

    ShowPillarOnWindow(drawList, botPose, IM_COL32(0, 120, 220, 100), offset);

    std::vector<Node<Pose2D>*> nodes = graph->getNodes();
    /*
    if (nodes.size() > 1) {
	std::cout << nodes.size() << std::endl; // 10
	std::cout << nodes[1]->getData().getX() << std::endl; // 0
    }
    */
    for (uint16_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {
	Pose2D position = nodes[nodeIndex]->getData();
	
	ImVec2 center = ImVec2(offset.x + position.getX() * SCREEN_SCALE, offset.y - position.getY() * SCREEN_SCALE);
	float radius = BOT_RADIUS / 2.0 * SCREEN_SCALE;
	DrawCircle(drawList, center, radius, IM_COL32(120, 120, 0, 200));
	// draw a line from every node to the adjacent yes we will double count draws with this
	//  std::vector<Node<Pose2D>*> adjacent = getAdj(nodes[nodeIndex]);
	// for (uint16_t connected = 0; connected < )

    }
/*
    if (path.size() > 0) {
	std::cout << "PATH SIZE: " << path.size() <<  " PATH 0 Y " << path[0].getY() << std::endl; 
    }
    */
    for (uint8_t i = 1; i < path.size(); i++) {
	ImVec2 p1 = coordsToScreen(offset, path[i - 1].getX(), path[i - 1].getY());
	ImVec2 p2 = coordsToScreen(offset, path[i].getX(), path[i].getY());
	drawList->AddLine(p1, p2, IM_COL32(100, 100, 100, 100), 2); 
//	std::cout << "p1 x: " << p1.x << ". p1 y: " << p1.y << std::endl;
    }


    pillarsMutex->unlock();

    ImGui::End();
    // std::cout << "Ended" << std::endl;
}

void addToQueue(std::string message) {
    // std::cout << message << std::endl;
  sendQueue.push(message);
}

void sendAngleToQueue(int16_t angle) {
  char buff[8];
    sprintf(buff, "t%03d", angle);

  
 // std::cout << std::string(buff) << std::endl;
  sendQueue.push(std::string(buff));
}

void sendDistanceToQueue(uint16_t angle) {
  char buff[8];
    sprintf(buff, "r%03d", angle);
 // std::cout << std::string(buff) << std::endl;
  sendQueue.push(std::string(buff));
}

bool validLocationForNode(std::vector<Pillar> pillars, uint8_t desired, Pose2D location) {
   for (uint8_t i = 0; i < pillars.size(); i++) {
	if (pillars[i].getPose().distanceTo(location) < pillars[i].getRadius() + BOT_RADIUS) {
	    return false;
	}
    }

    return true;
}


void discretizeGraph(std::vector<Pillar>& pillars, std::mutex& fieldMutex, uint8_t desired, Pillar botPose, Graph<Pose2D>* graph) {
    fieldMutex.lock();
    graph->addNode(new Node<Pose2D>(botPose.getPose()));
    // std::vector<Node<Pillar>*> nodes;
    for (uint8_t currentPillar = 0; currentPillar < pillars.size(); currentPillar++) {
	if (currentPillar != desired) {
	    double magnitude = pillars[currentPillar].getRadius() + BOT_RADIUS;
	    for (int i = 1; i < 3; i++) { 
		for (double angle = 0; angle < 361; angle += 35) {
		    double radian = angle * M_PI / 180.0;
		    Pose2D attemptAdd = Pose2D::fromPolar(magnitude * i, radian);

		    attemptAdd.plus(pillars[currentPillar].getPose());

		    if (validLocationForNode(pillars, desired, attemptAdd)) {
			// add to list
			Node<Pose2D>* toAdd = new Node<Pose2D>(attemptAdd);
			
			graph->addNode(toAdd);
			// nodes = graph.getNodes();
		    }
		}
	    }
	}	
	graph->addNode(new Node<Pose2D>(botPose.getPose()));
    }
    
    // graph->setHead(graph->getNodes().size() - 1);
    // std::cout << "X: " << pillars[desired].getX() << std::endl;
    graph->addNode(new Node<Pose2D>(pillars[desired].getPose()));

    // std::cout << "SIZE: " << graph->getNodes().size() << std::endl;
    // std::cout << " X: " << graph->getNodes()[1]->getData().getX() << std::endl; // 8.7

    fieldMutex.unlock();
}

bool lineIntersectsCircle(double cx, double cy, double r, double x1, double y1, double x2, double y2) {
    // Calculate the line direction vector
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Calculate the projection of the circle center onto the line
    double t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);

    // Find the closest point on the line to the circle center
    double closestX = x1 + t * dx;
    double closestY = y1 + t * dy;

    // Calculate the distance from the circle center to the closest point
    double distX = closestX - cx;
    double distY = closestY - cy;
    double distanceToLine = std::sqrt(distX * distX + distY * distY);

    // Check if the distance is less than or equal to the radius
    return distanceToLine <= r;
}

bool lineIntersectsCircle(Pillar p1, Pose2D one, Pose2D two) {
    return lineIntersectsCircle(p1.getX(), p1.getY(), p1.getRadius() + BOT_RADIUS, one.getX(), one.getY(), two.getX(), two.getY());
}

void weightGraph(Graph<Pose2D>* graph, std::vector<Pillar>& pillars, std::mutex& fieldMutex, uint8_t desired, Pillar botPose) {
    fieldMutex.lock();
    std::vector<Node<Pose2D>*> nodes = graph->getNodes();
    for (uint16_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {
	for (uint16_t nodeIndexTwo = 0; nodeIndexTwo < nodes.size(); nodeIndexTwo++) {
	    if (nodeIndex != nodeIndexTwo) {
		Pose2D positionOne = nodes[nodeIndex]->getData();
		Pose2D positionTwo = nodes[nodeIndexTwo]->getData();
		double length = positionOne.distanceTo(positionTwo);
		double dy = (positionTwo.getY() - positionOne.getY());
		double dx = (positionTwo.getX() - positionTwo.getY());
		bool gotThrough = true;

		for (uint8_t pillarIndex = 0; pillarIndex < pillars.size(); pillarIndex++) {
		    if (pillarIndex != desired) {
			if (lineIntersectsCircle(pillars[pillarIndex], positionOne, positionTwo)) {			
			    // uh oh we hit the circle
			    gotThrough = false;
			}
		    }
		}

		if (gotThrough) {
		    // add a weight between nodes[nodeIndex] and nodes[nodeIndexTwo]
		    graph->addConnection(nodes[nodeIndex], nodes[nodeIndexTwo], length);
		    // print out attempt to add
		    // std::cout << "attempting to connnect: " << nodes[nodeIndex]->getData().getX() << " with: " << nodes[nodeIndexTwo]->getData().getX() << std::endl;
		}
	    }
	}
    }
    fieldMutex.unlock();
}

void pathToRoutine(std::vector<Pose2D> path, std::vector<Move>& routine) {
    // gurantee that we can look back.
    std::cout << "path size: " << path.size() << std::endl;
    std::cout << "path 0: " << path[0].getX() << ". " << path[0].getY() << std::endl;
    std::cout << "path 1: " << path[1].getX() << ". " << path[1].getY() << std::endl;
    std::cout << "path 2: " << path[2].getX() << ". " << path[2].getY() << std::endl;

    // BUFFER OVERFLOW
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
	std::cout << "angle: " << angle << ". magnitude: " << magnitude << std::endl;
    }
}

std::string parsePathIntoRoutine(std::vector<Pose2D> path) {
    std::stringstream toSend;

    std::vector<Move> routine;
    // std::cout << "routine length: " << routine.size() << std::endl;
    pathToRoutine(path, routine);

    toSend << "R ";

    for (uint8_t i = 0; i < routine.size(); i++) {
	static char buffer[50];
	// sprintf(buffer, "p %0.3f %0.3f " , path[i].getX(), path[i].getY());
	// // routine[i] is all 0
	sprintf(buffer, "p %0.3f %d ", routine[i].quantity, routine[i].type);
	toSend << std::string(buffer);
    }

    toSend << " R";

    
    return toSend.str();
}

int main() {
  if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1880, 900, "Roomba Dashboard", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    setupImGui(window);

    Pillar botPose(0, 0, 0, BOT_RADIUS);

    uint8_t desired = 0;

    float angleSend = 0;
    int driveForward = 0;

    std::vector<Pillar> pillars;
    std::mutex pillarsMutex;
    Graph<Pose2D>* graph = new Graph<Pose2D>();
    std::vector<Pose2D> path;


    // Pose2D toAdd(0, 0, 0);

    // graph.addNode(new Node<Pose2D>(toAdd));


    // connectTCP(pillars, pillarsMutex, desired);
    std::thread tcpConnect(connectTCP, std::ref(pillars), std::ref(pillarsMutex), std::ref(desired), std::ref(botPose));
    
    // Pillar pillar(3.2, 9.1, 2.2, 6.7);
    // pillars.push_back(/**Pillar(5.0f, 5.0f, 0.0f, 2.5)*/ pillar);
/*
    std::cout << "pillar 0 x" << pillar.getX() << " pillar 0 y "<< pillar.getY() << " . radius: " << pillar.getRadius()  << std::endl;

    std::cout << "pillar 1 x " << pillars[0].getX() << ". pillar 1 y " << pillars[0].getY() << ". radius: " << pillars[0].getRadius() << std::endl; 
*/
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
	// bool open;
	// ImGui::ShowDemoWindow(&open);
	

	ShowFieldWindow(std::ref(pillars), &pillarsMutex, botPose, graph, path);
	// std::cout << "About to begin" << std::endl;
	
        // Your ImGui code here
        ImGui::Begin("Control Panel");

// 	std::cout << "began" << std::endl;

        //ImGui::Text("This is some text");
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

	if(ImGui::Button("Quit all")) {
	    addToQueue("q");
	}

	if (ImGui::Button("Generate path")) {
	    pillarsMutex.lock();
	    std::vector<Node<Pose2D>*> pathNodes = graph->Dijkstra(graph->getNodes().back(), graph->getNodes()[desired - 1]);
	   //  std::cout << "PATH NODE SIZE: " << pathNodes.size() << std::endl;
	    path.push_back(botPose.getPose());
	    for (uint8_t i = 0; i < pathNodes.size(); i++) {
		path.push_back(pathNodes[i]->getData());
		std::cout << path[i].getX() << ", " << path[i].getY() << std::endl;
	    }
	    std::cout << path[path.size() - 1].getX() << ", " << path[path.size() - 1].getY() << std::endl; 
	   
	    std::vector<std::vector<unsigned int>> adjacencyMatrix = graph->getAdjacencyList();

	    for (int i = 0; i < adjacencyMatrix.size(); i++) {
		for (int j = 0; j < adjacencyMatrix[i].size(); j++) {
		    std::cout << ", " << adjacencyMatrix[i][j]; 
		}
		std::cout << std::endl;
	    }

	    pillarsMutex.unlock();
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
	    delete graph;
	    graph = new Graph<Pose2D>();
	    // std::cout << "HUH: " << pillars[0].getX() << std::endl;
	   discretizeGraph(std::ref(pillars), pillarsMutex, desired, botPose, graph);
	}

	if (ImGui::Button("Weight")) {
	    weightGraph(graph, pillars, pillarsMutex, desired, botPose);
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
