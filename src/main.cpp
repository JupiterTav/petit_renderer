#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Camera.hpp"
#include "ImGuiUI.hpp"
#include "RendererContext.hpp"

#include <iostream>

using namespace renderer;
using namespace ui;

constexpr int WINDOW_WIDTH{1920};
constexpr int WINDOW_HEIGHT{1080};

float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void glfwMouseCallback(GLFWwindow *window, double xpos, double ypos);
void glfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos,
                    Camera &camera);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset,
                     Camera &camera);

void processInput(GLFWwindow *window, Camera &camera);

void glfwErrorCallback(int error, const char *description) {
  std::fprintf(stderr, "GLFW::Error::%d: %s\n", error, description);
}

/**
 *  @brief Build the main menu of the application
 * */
void imGuiDrawFrame(RendererContext &context) {
  ImGui::Begin("Petit Renderer");
  ImGui::Text("OpenGL: %s",
              reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  ImGui::Text("Position: (%.2f, %.2f, %.2f)", context.camera.position_.x,
              context.camera.position_.y, context.camera.position_.z);
  ImGui::Text("Rotation: yaw %.1f, pitch %.1f", context.camera.yaw_,
              context.camera.pitch_);
  ImGui::End();
}

int main(int argc, char *argv[]) {
  RendererContext context;

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
  glfwSetFramebufferSizeCallback(context.window, framebuffer_size_callback);
  glfwSetWindowUserPointer(context.window, &context);
  glfwSetCursorPosCallback(context.window, glfwMouseCallback);
  glfwSetScrollCallback(context.window, glfwScrollCallback);
  glfwSetErrorCallback(glfwErrorCallback);

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

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(context.window, context.camera);

    ui.newFrame();
    imGuiDrawFrame(context);

    glClear(GL_COLOR_BUFFER_BIT);

    ui.renderFrame();

    glfwSwapBuffers(context.window);
  }

  glfwDestroyWindow(context.window);

  return 0;
}

/**
 * @function process all input: query GLFW whether relevant keys are
 * pressed/released this frame and react accordingly
 */
void processInput(GLFWwindow *window, Camera &camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const ImGuiIO &io = ImGui::GetIO();
  if (!io.WantCaptureMouse && !io.WantCaptureKeyboard) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.processKeyboard(RIGHT, deltaTime);
  }
}

/**
 * @function glfw: whenever the window size changed (by OS or user resize) this
 callback function executes
*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: trampolines that look up the camera passed in through the user pointer
// ---------------------------------------------------------------------------
void glfwMouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
  RendererContext *context =
      static_cast<RendererContext *>(glfwGetWindowUserPointer(window));
  mouse_callback(window, xposIn, yposIn, context->camera);
}

void glfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  RendererContext *context =
      static_cast<RendererContext *>(glfwGetWindowUserPointer(window));
  scroll_callback(window, xoffset, yoffset, context->camera);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn,
                    Camera &camera) {
  // let ImGui own the mouse while the cursor is over its windows; reset the
  // reference point so re-entering the viewport doesn't jerk the camera
  if (ImGui::GetCurrentContext() != nullptr &&
      ImGui::GetIO().WantCaptureMouse) {
    firstMouse = true;
    return;
  }

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset,
                     Camera &camera) {
  // don't zoom the camera while ImGui is scrolling its own windows
  if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureMouse)
    return;

  camera.processMouseScroll(static_cast<float>(yoffset));
}
