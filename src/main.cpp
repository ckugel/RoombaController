#include "../include/imgui/imgui.h"
// #include "../include/imgui/imgui_demo.cpp"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Graph.h"
#include "Pillar.h"
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
#include "Pose2D.h"

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
  strftime(name_buff, sizeof(name_buff), "log_%Y%m%d_%H%M%S", localtime(&now));
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

void ShowPillarWindow(const std::vector<Pillar>& pillars) {
  ImGui::Begin("Field");


}


void addToQueue(string message) {
  sendQueue.add(message);
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello, ImGui!", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    setupImGui(window);

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

        // Your ImGui code here
        ImGui::Begin("Control Panel");
        //ImGui::Text("This is some text");
	ImGui::Button("Forward", );
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
