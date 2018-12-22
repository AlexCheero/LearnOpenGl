#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
	GLuint _shaderProgram;

	GLuint CreateShaderObjectFromFile(const GLchar* filePath, GLenum type);
	GLuint CreateShaderObjectFromSource(GLenum type, const GLchar* shaderSource);
	void LogShaderCompilationError(GLenum type, GLuint shader) const;
	void CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader);

public:
	Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	void Use() const { glUseProgram(_shaderProgram); }
};

