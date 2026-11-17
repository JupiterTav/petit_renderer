#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ImGuiUI.hpp"
#include "RendererContext.hpp"

#include <iostream>

constexpr int WINDOW_WIDTH{1920};
constexpr int WINDOW_HEIGHT{1080};

using namespace renderer;
using namespace ui;

void glfwErrorCallback(int error, const char *description) {
  std::fprintf(stderr, "GLFW::Error::%d: %s\n", error, description);
}

void imGuiDrawFrame(RendererContext &context) {
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

  ImGuiUI ui(context.window);
  while (!glfwWindowShouldClose(context.window)) {
    glfwPollEvents();
    ui.newFrame();
    imGuiDrawFrame(context);

    glClear(GL_COLOR_BUFFER_BIT);

    ui.renderUpdate();

    glfwSwapBuffers(context.window);
  }

  glfwDestroyWindow(context.window);

  return 0;
}
