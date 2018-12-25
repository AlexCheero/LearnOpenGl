#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "Shader.h"
#include "Log.h"

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

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int PrepareData(std::vector<float> vertexData, std::vector<unsigned int> indices = {})
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	GLsizeiptr vertsSize = sizeof(*vertexData.data()) * vertexData.size();
	glBufferData(GL_ARRAY_BUFFER, vertsSize, vertexData.data(), GL_STATIC_DRAW);

	if (indices.size() > 0)
	{
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		GLsizeiptr indSize = sizeof(*indices.data()) * indices.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, indices.data(), GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Unbind();

	return VAO;
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
	return true;
}

int main(int argc, char *argv[])
{
	if (!Init(argv[0]))
		return -1;

	Shader shader(exeRoot + "Shaders\\DefaultVertexShader.glsl", exeRoot + "Shaders\\DefaultFragmentShader.glsl");

	unsigned int VAO = PrepareData(
		{
			// positions         // colors
			0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
		});

	//-----------------Prepare texture------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	std::string imagePath = exeRoot + "Textures\\container.jpg";
	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		Log() << "Failed to load texture" << '\n';

	stbi_image_free(data);
	//--------------------------------------------------------

	glBindVertexArray(VAO);
	shader.Use();
	int vertexColorLocation = shader.GetUniformLocation("ourColor");
	int vertexOffsetLocation = shader.GetUniformLocation("offset");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUniform4f(vertexColorLocation, 1 - greenValue, greenValue, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}