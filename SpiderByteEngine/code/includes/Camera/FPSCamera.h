//@TODO: This is not my code but created by JoeyDeVries
//However we eventually need to change this into a struct to
//match my style of programming NO OOP Here!!
#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    DOWN,
    UP,
  };

class FPSCamera
{
 public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  float Yaw = -90.0f;
  float Pitch = 0.0f;
  float MovementSpeed = 2.5f;
  float MouseSensitivity = 0.1f;
  float Zoom = 45.0f;

  FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
  
  FPSCamera(float posX, float posY, float posZ, float upX, float upY,
	    float upZ, float yaw, float pitch);

  glm::mat4 GetViewMatrix();
  void ProcessKeyboard(Camera_Movement direction, float DeltaTime);
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
  void ProcessMouseScroll(float yoffset);

 private:
  void UpdateCameraVectors();
};

#endif
