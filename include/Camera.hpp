#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };
/**
 * @brief Default Configuration Camera Value
 * */
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

namespace renderer {

class Camera {
public:
  // camera Attributes
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 worldUp_;
  /** @brief Euler Angles
   * YAW -> Rotation around y-axis;
   * Pitch -> Rotation around x-axis;
   */
  float yaw_;
  float pitch_;
  // camera options
  float movementSpeed_;
  float mouseSensitivity_;
  float zoom_;

  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);
  glm::mat4 getViewMatrix();
  void processKeyboard(Camera_Movement direction, float deltaTime);
  void processMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);
  void processMouseScroll(float yoffset);

private:
  void updateCameraVectors();
};
} // namespace renderer
