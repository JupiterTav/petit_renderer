#pragma once
#include <GLFW/glfw3.h>

#include "Camera.hpp"

namespace renderer {

struct RendererContext {
  GLFWwindow *window;
  Camera camera{glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                YAW, PITCH};
};

}; // namespace renderer
