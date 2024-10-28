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

#define ADDRESS "127.0.0.1" // "192.168.1.1"
#define PORT 65432
#define SCREEN_SCALE 2.0f
#define BOT_RADIUS 10

std::queue<std::string> sendQueue;
std::mutex queueMutex;
std::condition_variable sendCondition;


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
		break;
	    case 'd':
		{
			uint8_t readAble;
			if (stream >> readAble) {
			    desired = readAble;
			}
			else {
			    logFile << "Could not parse stream for: d" << std::endl;
			}
		}
		break;
	    case 'o':
		    {	fieldMutex.lock();
			field.push_back(Pillar::parseFromStream(stream));
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
}

void ShowPillarOnWindow(ImDrawList* drawList, Pillar pillar, ImU32 color, ImVec2 offset) {
    	ImVec2 center = ImVec2(offset.x + pillar.getX() * SCREEN_SCALE, offset.y + pillar.getY() * SCREEN_SCALE);
	float radius = pillar.getRadius() * SCREEN_SCALE;
	// std::cout << "haven't drawn yet" << std::endl;
	DrawCircle(drawList, center, radius, color);
}


void ShowFieldWindow(std::vector<Pillar> pillars, std::mutex* pillarsMutex, Pillar botPose, Graph<Pose2D>& graph) {
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

    std::vector<Node<Pose2D>*> nodes = graph.getNodes();
    for (uint16_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {
	Pose2D position = nodes[nodeIndex]->getData();
	
	ImVec2 center = ImVec2(offset.x + position.getX() * SCREEN_SCALE, offset.y + position.getY() * SCREEN_SCALE);
	float radius = BOT_RADIUS / 2.0 * SCREEN_SCALE;
	DrawCircle(drawList, center, radius, IM_COL32(120, 120, 0, 200));
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


Graph<Pose2D> discretizeGraph(std::vector<Pillar> pillars, std::mutex& fieldMutex, uint8_t desired, Pillar botPose) {
    Graph<Pose2D> graph;
    fieldMutex.lock();
    // std::vector<Node<Pillar>*> nodes;
    for (uint8_t currentPillar = 0; currentPillar < pillars.size(); currentPillar++) {
	if (currentPillar != desired) {
	    double magnitude = pillars[currentPillar].getRadius() + BOT_RADIUS;
	    
	    for (double angle = 0; angle < 361; angle += 50) {
		double radian = angle * M_PI / 180.0;
		Pose2D attemptAdd = Pose2D::fromPolar(magnitude, radian);
		if (validLocationForNode(pillars, desired, attemptAdd)) {
		    // add to list
		    Node<Pose2D>* toAdd = new Node<Pose2D>(attemptAdd);
		    
		    graph.addNode(toAdd);
		    // nodes = graph.getNodes();
		}
	    }
	}	
    }

    graph.addNode(new Node<Pose2D>(botPose.getPose()));
    graph.addNode(new Node<Pose2D>(pillars[desired].getPose()));

    fieldMutex.unlock();
    return graph;    
}

void weightGraph(Graph<Pose2D>& graph, std::vector<Pillar>& pillars, std::mutex& fieldMutex, uint8_t desired, Pillar botPose) {
    fieldMutex.lock();
    std::vector<Node<Pose2D>*> nodes = graph.getNodes();
    for (uint16_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {
	for (uint16_t nodeIndexTwo = 0; nodeIndexTwo < nodes.size(); nodeIndexTwo++) {
	    if (nodeIndex != nodeIndexTwo) {
		Pose2D positionOne = nodes[nodeIndex]->getData();
		Pose2D positionTwo = nodes[nodeIndex]->getData();
		double length = positionOne.distanceTo(positionTwo);
		double dy = (positionTwo.getY() - positionOne.getY()) / length;
		double dx = (positionTwo.getX() - positionTwo.getY()) / length;
		bool gotThrough = true;

		for (uint8_t pillarIndex = 0; pillarIndex < pillars.size(); pillarIndex++) {
		    if (pillarIndex != desired) {
			double t = dx * (pillars[pillarIndex].getX() - positionOne.getX()) + dy * (pillars[pillarIndex].getY() - positionOne.getY());
			double Ex = t * dx + positionOne.getX();
			double Ey = t * dy + positionOne.getY();

			double L = pow(Ex - pillars[pillarIndex].getX(), 2) + pow(Ey - pillars[pillarIndex].getY(), 2); 

			if (L < pow(pillars[pillarIndex].getRadius() + BOT_RADIUS, 2)) {
			    // uh oh we hit the circle
			    gotThrough = false;
			}
		    }
		}

		if (gotThrough) {
		    // add a weight between nodes[nodeIndex] and nodes[nodeIndexTwo]
		    graph.addConnection(nodes[nodeIndex], nodes[nodeIndexTwo], length);
		}
	    }
	}
    }
    fieldMutex.unlock();
}


std::string parsePathIntoRoutine(std::vector<Pose2D> path) {
    //TODO: IMPLEMENT
    return "";
}

int main() {
  if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1880, 900, "Roomba Dashboard", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    setupImGui(window);

    Pillar botPose(0, 0, 0);

    uint8_t desired = 0;

    float angleSend = 0;
    int driveForward = 0;

    std::vector<Pillar> pillars;
    std::mutex pillarsMutex;
    Graph<Pose2D> graph;
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
	

	ShowFieldWindow(pillars, &pillarsMutex, botPose, graph);
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
	    std::vector<Node<Pose2D>*> pathNodes = graph.Dijkstra(graph.getNodes().back());
	    for (uint8_t i = 0; i < pathNodes.size(); i++) {
		path.push_back(pathNodes[i]->getData());
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
	   graph = discretizeGraph(pillars, pillarsMutex, desired, botPose);
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
