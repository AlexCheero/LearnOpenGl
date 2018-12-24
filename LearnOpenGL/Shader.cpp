#include "Shader.h"

#include <iostream>
#include <fstream>

Shader::Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath)
{
	GLuint vertexShader = CreateShaderObjectFromFile(vertexSourcePath.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateShaderObjectFromFile(fragmentSourcePath.c_str(), GL_FRAGMENT_SHADER);

	CreateShaderProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glLinkProgram(_shaderProgram);

	GLint success;
	GLchar infoLog[logLength];
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_shaderProgram, logLength, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

GLuint Shader::CreateShaderObjectFromFile(const GLchar* filePath, GLenum type)
{
	std::ifstream file(filePath);
	std::string shaderSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();
	return CreateShaderObjectFromSource(type, shaderSource.c_str());
}

GLuint Shader::CreateShaderObjectFromSource(GLenum type, const GLchar* shaderSource)
{
	GLint success;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
		LogShaderCompilationError(type, shader);

	return shader;
}

void Shader::LogShaderCompilationError(GLenum type, GLuint shader) const
{
	const char* shaderType;
	switch (type)
	{
	case GL_VERTEX_SHADER:
		shaderType = "VERTEX";
		break;
	case GL_FRAGMENT_SHADER:
		shaderType = "FRAGMENT";
		break;
	default:
		char buffer[10];
		_itoa_s(type, buffer, 10);
		shaderType = buffer;
		break;
	}
	GLchar infoLog[logLength];
	glGetShaderInfoLog(shader, logLength, nullptr, infoLog);
	std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
}