#include "Camera.h"
#include <GLFW/glfw3.h>

#include <glm/detail/func_geometric.inl>
#include <glm/detail/func_trigonometric.inl>

void Camera::Move(EMoveDirection direction, GLFWwindow& window, float delta)
{
	float speedDelta = camSpeed * delta;
	switch (direction)
	{
	case EMoveDirection::Forward:
		cameraPos += speedDelta * cameraFront;
		break;
	case EMoveDirection::Backward:
		cameraPos -= speedDelta * cameraFront;
		break;
	case EMoveDirection::Right:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speedDelta;
		break;
	case EMoveDirection::Left:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speedDelta;
		break;
	default: 
		break;
	}
}

void Camera::Rotate(float x, float y)
{
	static float lastMouseX = ViewportWidth / 2;
	static float lastMouseY = ViewportHeight / 2;
	static bool firstMouse = true;
	if (firstMouse)
	{
		lastMouseX = x;
		lastMouseY = y;
		firstMouse = false;
	}

	float xoffset = x - lastMouseX;
	float yoffset = lastMouseY - y;
	lastMouseX = x;
	lastMouseY = y;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > MaxPitch)
		pitch = MaxPitch;
	if (pitch < -MaxPitch)
		pitch = -MaxPitch;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void Camera::SetFov(float newFov)
{
	if (fov >= MinFov && fov <= MaxFov)
		fov -= newFov;
	if (fov <= MinFov)
		fov = MinFov;
	if (fov >= MaxFov)
		fov = MaxFov;
}
