#include "../include/imgui/imgui.h"
// #include "../include/imgui/imgui_demo.cpp"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Graph.h"
#include "Pillar.h"
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>



// connect to Roomba server
void connectTCP() {
 int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(288);
  serverAddress.sin_addr.s_addr = inet_addr("192.168.1.1");

  int status = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  // maybe make thread here


  const char* message = "Handshake";
  send(clientSocket, message, strlen(message), 0);

  // expect a "Handshake" response to be echoed
  read();


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

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello, ImGui!", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    setupImGui(window);

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
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some text");
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

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
