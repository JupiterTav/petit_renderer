#include "Camera.hpp"

namespace renderer {

/**
 *  @function constructor with vectors
 * Build Position, WorldUp, Yay and Pitch.
 * Call updateCameraVectors() at the end
 * */
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED),
      mouseSensitivity_(SENSITIVITY), zoom_(ZOOM) {
  position_ = position;
  worldUp_ = up;
  yaw_ = yaw;
  pitch_ = pitch;
  updateCameraVectors();
}
/** @function
 * constructor with scalar values
 */
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED),
      mouseSensitivity_(SENSITIVITY), zoom_(ZOOM) {
  position_ = glm::vec3(posX, posY, posZ);
  worldUp_ = glm::vec3(upX, upY, upZ);
  yaw_ = yaw;
  pitch_ = pitch;
  updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position_, position_ + front_, up_);
}

/**
 * @function processes input received from any keyboard-like input system.
 * Accepts input parameter in the form of camera defined ENUM (to abstract it
 * from windowing systems)
 */
void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = movementSpeed_ * deltaTime;
  if (direction == FORWARD)
    position_ += front_ * velocity;
  if (direction == BACKWARD)
    position_ -= front_ * velocity;
  if (direction == LEFT)
    position_ -= right_ * velocity;
  if (direction == RIGHT)
    position_ += right_ * velocity;
}

/**
 * @function processes input received from a mouse input system. Expects the
 * offset value in both the x and y direction.
 */
void Camera::processMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch) {
  xoffset *= mouseSensitivity_;
  yoffset *= mouseSensitivity_;

  yaw_ += xoffset;
  pitch_ += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (pitch_ > 89.0f)
      pitch_ = 89.0f;
    if (pitch_ < -89.0f)
      pitch_ = -89.0f;
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

/**
 * @function processes input received from a mouse scroll-wheel event.
 * Only requires input on the vertical wheel-axis
 */
void Camera::processMouseScroll(float yoffset) {
  zoom_ -= (float)yoffset;
  if (zoom_ < 1.0f)
    zoom_ = 1.0f;
  if (zoom_ > 45.0f)
    zoom_ = 45.0f;
}

/**
 * @function calculates the front vector from the Camera's Euler Angles
 *
 * Calculates the front vector throw the sine and cosine of the yaw and pitch
 * then calculate the Right and the Up vectors using Cross product
 * (Gram-Schmidt Algorithm)
 *
 * obs:  normalize the vectors, because their length gets closer to 0
 * the more you look up or down which results in slower movement.
 */
void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front);
  right_ = glm::normalize(glm::cross(front_, worldUp_));

  up_ = glm::normalize(glm::cross(right_, front_));
}
}; // namespace renderer
