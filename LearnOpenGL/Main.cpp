#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"
#include "Log.h"

#include "Camera.h"

struct Color
{
	float r, g, b, a;
};

int ViewportWidth = 800;
int ViewportHeight = 600;
Color ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
std::string exePath;
std::string exeRoot;
GLFWwindow* window;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera mainCamera;

void InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* InitWindow(int width, int height, const char* caption)
{
	GLFWwindow* window = glfwCreateWindow(width, height, caption, nullptr, nullptr);
	if (window == nullptr)
	{
		Log() << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	return window;
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	mainCamera.Rotate(xpos, ypos);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera.SetFov(yoffset);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera.Move(EMoveDirection::Forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera.Move(EMoveDirection::Backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.Move(EMoveDirection::Left, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.Move(EMoveDirection::Right, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		mainCamera.Move(EMoveDirection::Down, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		mainCamera.Move(EMoveDirection::Up, deltaTime);
}

void Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int PrepareVAO()
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	return VAO;
}

template <class T>
unsigned int PrepareBufferObject(std::vector<T> data, GLenum target, GLenum usage = GL_STATIC_DRAW)
{
	unsigned int BufferObject;
	glGenBuffers(1, &BufferObject);
	glBindBuffer(target, BufferObject);
	GLsizeiptr dataSize = sizeof(*data.data()) * data.size();
	glBufferData(target, dataSize, data.data(), usage);
	return BufferObject;
}

unsigned int PrepareTexture(const char* path, int filtering)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrComponents;
	std::string imagePath = exeRoot + path;
	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		Log() << "Failed to load texture" << '\n';

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return texture;
}

bool Init(char* path)
{
	exePath = std::string(path);
	exeRoot = exePath.substr(0, exePath.rfind('\\') + 1);

	InitGLFW();
	window = InitWindow(ViewportWidth, ViewportHeight, "LearnOpenGL");
	if (window == nullptr)
		return false;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log() << "Failed to initialize GLAD" << '\n';
		return false;
	}

	glViewport(0, 0, ViewportWidth, ViewportHeight);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	return true;
}

int main(int argc, char *argv[])
{
	if (!Init(argv[0]))
		return -1;

	Shader objectShader(exeRoot + "Shaders\\1.colors.vs", exeRoot + "Shaders\\1.colors.fs");
	Shader lampShader(exeRoot + "Shaders\\1.lamp.vs", exeRoot + "Shaders\\1.lamp.fs");

	std::vector<float> verticies =
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//------------Object VAO------------
	unsigned int VAO = PrepareVAO();
	unsigned int VBO = PrepareBufferObject(verticies, GL_ARRAY_BUFFER);
	unsigned int stride = 8 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	Unbind();
	//----------------------------------

	//------------Light VAO------------
	unsigned int lightVAO = PrepareVAO();
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	Unbind();
	//---------------------------------

	//-------------Textures stuff-------------
	stbi_set_flip_vertically_on_load(true);

	int diffuseMap = PrepareTexture("Textures\\container2.png", GL_REPEAT);
	int specularMap = PrepareTexture("Textures\\lighting_maps_specular_color.png", GL_REPEAT);
	int emissionMap = PrepareTexture("Textures\\matrix.jpg", GL_REPEAT);

	objectShader.Use();
	//define what sampler corresponds to what texture
	glUniform1i(objectShader.GetUniformLocation("material.diffuse"), 0);
	glUniform1i(objectShader.GetUniformLocation("material.specular"), 1);
	glUniform1i(objectShader.GetUniformLocation("material.emission"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	//----------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------Light position-----------
		//glm::vec3 lightPos(0.5f, 0.3f, 2.0f);
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		//------------------------------------

//------------Draw Object------------
		glBindVertexArray(VAO);
		objectShader.Use();
		glUniform3f(objectShader.GetUniformLocation("lightColor"), 1.0f, 1.0f, 1.0f);

		glUniform3f(objectShader.GetUniformLocation("material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(objectShader.GetUniformLocation("material.shininess"), 32.0f);

		glm::mat4 view = mainCamera.GetView();

		glm::vec3 viewLightPos = glm::vec3(view * glm::vec4(lightPos, 1.0f));
		glUniform3fv(objectShader.GetUniformLocation("light.position"), 1, glm::value_ptr(viewLightPos));

		glUniform3f(objectShader.GetUniformLocation("light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(objectShader.GetUniformLocation("light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(objectShader.GetUniformLocation("light.specular"), 1.0f, 1.0f, 1.0f);

		//------------Camera Transformations------------
		glUniformMatrix4fv(objectShader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.GetProjection()));
		//----------------------------------------------

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3());
		glm::mat4 modelView = view * model;
		glUniformMatrix4fv(objectShader.GetUniformLocation("modelView"), 1, GL_FALSE, glm::value_ptr(modelView));

		glm::mat3 normalMartix = glm::transpose(glm::inverse(modelView));
		glUniformMatrix3fv(objectShader.GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMartix));

		glDrawArrays(GL_TRIANGLES, 0, 36);
//-----------------------------------

//------------Draw Lamp------------
		glBindVertexArray(lightVAO);
		lampShader.Use();
		//------------Camera Transformations------------
		glUniformMatrix4fv(lampShader.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lampShader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.GetProjection()));
		//----------------------------------------------

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(lampShader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
//---------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}