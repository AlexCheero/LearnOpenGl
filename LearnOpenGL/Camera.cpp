#include "Camera.h"

void Camera::Move(EMoveDirection direction, float delta)
{
	float speedDelta = params.camSpeed * delta;
	switch (direction)
	{
	case EMoveDirection::Forward:
		cameraPos += speedDelta * cameraFront;
		break;
	case EMoveDirection::Backward:
		cameraPos -= speedDelta * cameraFront;
		break;
	case EMoveDirection::Right:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speedDelta;
		break;
	case EMoveDirection::Left:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speedDelta;
		break;
	default: 
		break;
	}
}

void Camera::Rotate(float x, float y)
{
	static float lastMouseX = params.width / 2;
	static float lastMouseY = params.height / 2;
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

	xoffset *= params.sensitivity;
	yoffset *= params.sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > CameraParams::MaxPitch)
		pitch = CameraParams::MaxPitch;
	if (pitch < -CameraParams::MaxPitch)
		pitch = -CameraParams::MaxPitch;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void Camera::SetFov(float fovDelta)
{
	if (params.fov >= CameraParams::MinFov && params.fov <= CameraParams::MaxFov)
		params.fov -= fovDelta;
	if (params.fov <= CameraParams::MinFov)
		params.fov = CameraParams::MinFov;
	if (params.fov >= CameraParams::MaxFov)
		params.fov = CameraParams::MaxFov;
}
