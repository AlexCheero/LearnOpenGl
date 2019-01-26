#pragma once
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

enum class EMoveDirection
{
	Forward,
	Backward,
	Right,
	Left,
	Up,
	Down
};

struct CameraParams
{
	friend class Camera;

private:
	static constexpr float MaxPitch = 89.0f;
	static constexpr float MaxFov = 90.0f;
	static constexpr float MinFov = 30.0f;
	static constexpr float DefaultFov = 45.0f;
	static constexpr float DefaultCamSpeed = 2.5f;
	static constexpr float DefaultSensitivity = 0.05f;
	static constexpr int DefaultViewportWidth = 800;
	static constexpr int DefaultViewportHeight = 600;
	static constexpr float DefaultNearClipPlane = 0.1f;
	static constexpr float DefaultFarClipPlane = 100.0f;

public:
	float fov = DefaultFov;
	float camSpeed = DefaultCamSpeed;
	float sensitivity = DefaultSensitivity;
	int width = DefaultViewportWidth;
	int height = DefaultViewportHeight;
	float nearClipPlane = DefaultNearClipPlane;
	float farClipPlane = DefaultFarClipPlane;
};

class Camera
{
	//create transform class and move it to transform
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//-----------------------------------------------

	static constexpr float DefaultYaw = -90.0f;
	static constexpr float DefaultPitch = 0.0f;

	float yaw = DefaultYaw;
	float pitch = DefaultPitch;

	CameraParams params;

public:
	Camera(CameraParams params = CameraParams()) {}
	void Move(EMoveDirection direction, float delta);
	void Rotate(float x, float y);
	void SetFov(float fovDelta);
	
	glm::mat4 GetView() const { return glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp); }
	glm::mat4 GetProjection() const
	{
		return glm::perspective(glm::radians(params.fov), static_cast<float>(params.width) / params.height, params.nearClipPlane, params.farClipPlane);
	}

	const glm::vec3& GetPosition() const
	{
		return cameraPos;
	}
};

