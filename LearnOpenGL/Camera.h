#pragma once
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

enum class EMoveDirection
{
	Forward,
	Backward,
	Right,
	Left
};

class Camera
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	const float MaxPitch = 89.0f;
	const float MaxFov = 90.0f;
	const float MinFov = 30.0f;
	const float DefaultFov = 45.0f;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float fov = DefaultFov;

	float camSpeed = 2.5f;

	int ViewportWidth = 800;
	int ViewportHeight = 600;

public:
	Camera();
	void Move(EMoveDirection direction, GLFWwindow& window, float delta);
	void Rotate(float x, float y);
	void SetFov(float fov);
};

