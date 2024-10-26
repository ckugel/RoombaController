#include "../include/imgui/imgui.h"
// #include "../include/imgui/imgui_demo.cpp"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
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
#include "Pose2D.hpp"
#include <sstream>
#include <iomanip>

#define ADDRESS "127.0.0.1" // "192.168.1.1"
#define PORT 65432

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


void readAndLog(int socket) {
  const uint16_t BUFF_SIZE = 1024;

  /*
  auto now = std::chrono::system_clock::now(); 
  std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

  std::tm* timeInfo = std::localtime(&timeNow);
  
  std::stringstream filename;
  filename << "log_" << std::put_time(timeInfo, "%Y%m%d_%H%M%S") << ".txt";
*/

  static char name_buff[32];
  time_t now = time(0);
  strftime(name_buff, sizeof(name_buff), "log/%Y%m%d_%H%M%S", localtime(&now));
  std::string str_name(name_buff);

  std::ofstream logFile(str_name);
  
  while (!(stopClient.load())) {

    char buff[BUFF_SIZE];

    // expect a "Handshake" response to be echoed
    uint16_t bytesRead = read(socket, buff, BUFF_SIZE);
    if (bytesRead > 0) {
      std::string response(buff, bytesRead);
      logFile << response << std::endl;
      if (response.find("quit") != std::string::npos) {
	std::cout << "exiting tcp server" << std::endl;
	stopClient.store(true);
      }
    }
    else {
      // sleep and wait
      // There should be an os signal to handle this
      // potentially fcntl
    }

    // no matter what we are going to log this in a file, however we should also update certain fields

  }

  logFile.close();

}

// connect to Roomba server
void connectTCP() {
 int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = inet_addr(ADDRESS);

  // https://beej.us/guide/bgnet/html/#blocking
  // fcntl(clientSocket, F_SETFL, O_NONBLOCK);
  int status = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  std::cout << "CONNECT" << std::endl;

  // maybe make thread here

  const char* message = "Handshake";
  send(clientSocket, message, strlen(message), 0);

  // spawn read and log thread here
  std::thread readThread(readAndLog, clientSocket);

  while(!stopClient.load()) {
	if (!sendQueue.empty()) {
	    std::string message = sendQueue.front();
	    if (message.compare("q") == 0) {
		stopClient.store(true);
	    }
	    sendQueue.pop();
	    send(clientSocket, message.c_str(), message.length(), 0);
	}
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

void ShowPillarWindow(std::vector<Pillar> pillars, std::mutex* pillarsMutex) {
  ImGui::Begin("Field");
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    float scale = 5.0f;
    ImVec2 offset = ImVec2(windowPos.x + 50, windowPos.y + 50);
    
    pillarsMutex->lock();

    for (Pillar pillar: pillars) {
	ImVec2 center = ImVec2(offset.x + pillar.getX() * scale, offset.y + pillar.getY() * scale);
	float radius = pillar.getRadius() * scale;
	DrawCircle(drawList, center, radius, IM_COL32(255, 100, 100, 255));
    }

    pillarsMutex->unlock();

    ImGui::End();
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

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Roomba Dashboard", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    setupImGui(window);

    float angleSend = 0;
    int driveForward = 0;

    std::vector<Pillar> pillars;
    std::mutex pillarsMutex;

    std::thread tcpConnect(connectTCP);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
	// bool open;
	// ImGui::ShowDemoWindow(&open);
	
	ShowPillarWindow(pillars, &pillarsMutex);

        // Your ImGui code here
        ImGui::Begin("Control Panel");
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

	ImGui::SliderAngle("Turn angle", &angleSend);

	if (ImGui::Button("Send turn")) {
	  sendAngleToQueue((int16_t) (angleSend * 180 / M_PI));
	}

	ImGui::SliderInt("Drive x cm", &driveForward, 0, 999);

	if (ImGui::Button("Drive x")) {
	    sendDistanceToQueue(driveForward);
	}

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
    }

    stopClient.store(true);
  
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
