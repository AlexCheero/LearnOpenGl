#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
	static const int logLength = 512;
private:
	GLuint _shaderProgram;

	GLuint CreateShaderObjectFromFile(const GLchar* filePath, GLenum type);
	GLuint CreateShaderObjectFromSource(GLenum type, const GLchar* shaderSource);
	void LogShaderCompilationError(GLenum type, GLuint shader) const;
	void CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);

public:
	Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath, const std::string& geometrySourcePath = "");
	void Use() const { glUseProgram(_shaderProgram); }
	int GetUniformLocation (const char* name) const { return glGetUniformLocation(_shaderProgram, name); }
};

