#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

constexpr int WINDOW_WIDTH{1920};
constexpr int WINDOW_HEIGHT{1080};

struct RendererContext {
  GLFWwindow *window;
};

void glfwErrorCallback(int error, const char *description) {
  std::fprintf(stderr, "GLFW::Error::%d: %s\n", error, description);
}

int imGuiInitialize(RendererContext &context) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  if (!ImGui_ImplGlfw_InitForOpenGL(context.window, true)) {
    std::fprintf(stderr, "Failed to initialize ImGui GLFW backend");
    glfwDestroyWindow(context.window);
    glfwTerminate();
    return -1;
  }

  if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
    std::fprintf(stderr, "Failed to initialize ImGui OpenGL");
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(context.window);
    glfwTerminate();
    return -1;
  }

  return 1;
}

void imGuiDrawFrame(RendererContext &context) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Petit Renderer");
  ImGui::Text("OpenGL: %s",
              reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  ImGui::End();
}

int main(int argc, char *argv[]) {
  RendererContext context;

  glfwSetErrorCallback(glfwErrorCallback);
  if (glfwInit() == GLFW_FALSE)
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  context.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                    "Petit Renderer", nullptr, nullptr);
  if (!context.window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(context.window);
  if (gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)) ==
      GLFW_FALSE) {
    std::fprintf(stderr, "Failed to load OpenGL functions\n");
    glfwDestroyWindow(context.window);
    glfwTerminate();
    return -1;
  }

  if (!imGuiInitialize(context)) {
    std::fprintf(stderr, "Failed to initialize Im GUI\n");
    glfwDestroyWindow(context.window);
    glfwTerminate();
    return -1;
  }

  while (!glfwWindowShouldClose(context.window)) {
    glfwPollEvents();
    imGuiDrawFrame(context);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(context.window);
  }

  glfwDestroyWindow(context.window);

  return 0;
}
