/**
 *  @file RAII wrapper to init, destroy, and render the imgui frame
 *  @date 2026-11-17
 *
 */

#include "ImGuiUI.hpp"

#include <iostream>
namespace ui {

/**
 *  @brief Constructor.
 *  @param window -> application window.
 *
 *  Checks if the initialization fails or not.
 *  Destroy it if failed to initialize
 *  something
 */
ImGuiUI::ImGuiUI(GLFWwindow *window) {
  IMGUI_CHECKVERSION();

  if (ImGui::CreateContext() == nullptr) {
    std::fprintf(stderr, "Failed to ImGui Create Context");
  }

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();
  if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
    std::fprintf(stderr, "Failed to initialize ImGui GLFW backend");
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
    std::fprintf(stderr, "Failed to initialize ImGui OpenGL");
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}

void ImGuiUI::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

/**
 * @brief Render the UI data of the frame.
 * @return void
 * */
void ImGuiUI::renderFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiUI::~ImGuiUI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
}; // namespace ui
