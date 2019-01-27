#pragma once
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Light
{
protected:
	//change to transform class when its done
	glm::vec3 coords;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
public:
	Light()
	{
		coords = glm::vec3(0.0f);
		ambient = glm::vec3(0.0f);
		diffuse = glm::vec3(0.0f);
		specular = glm::vec3(0.0f);
	}

	Light(glm::vec3 coord, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
	{
		coords = coord;
		ambient = amb;
		diffuse = diff;
		specular = spec;
	}

	virtual ~Light() = default;

	virtual void Apply(Shader shader, std::string uniformStructName)
	{
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".direction").c_str()), 1, glm::value_ptr(coords));
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".ambient").c_str()), 1, glm::value_ptr(ambient));
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".diffuse").c_str()), 1, glm::value_ptr(diffuse));
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".specular").c_str()), 1, glm::value_ptr(specular));
	}
};

class PointLight : public Light
{
	float constant;
	float linear;
	float quadratic;

public:
	PointLight()
	{
		constant = 0.0f;
		linear = 0.0f;
		quadratic = 0.0f;
	}

	PointLight(glm::vec3 coord, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cnst, float lin, float quad)
		: Light(coord, amb, diff, spec)
	{
		constant = cnst;
		linear = lin;
		quadratic = quad;
	}

	PointLight& operator=(const PointLight& other)
	{
		coords = other.coords;
		ambient = other.ambient;
		diffuse = other.diffuse;
		specular = other.specular;

		constant = other.constant;
		linear = other.linear;
		quadratic = other.quadratic;

		return *this;
	}

	void Apply(Shader shader, std::string uniformStructName) override
	{
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".position").c_str()), 1, glm::value_ptr(coords));
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".ambient").c_str()), 1, glm::value_ptr(ambient));
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".diffuse").c_str()), 1, glm::value_ptr(diffuse));
		glUniform3fv(shader.GetUniformLocation((uniformStructName + ".specular").c_str()), 1, glm::value_ptr(specular));

		glUniform1f(shader.GetUniformLocation((uniformStructName + ".constant").c_str()), constant);
		glUniform1f(shader.GetUniformLocation((uniformStructName + ".linear").c_str()), linear);
		glUniform1f(shader.GetUniformLocation((uniformStructName + ".quadratic").c_str()), quadratic);
	}
};
