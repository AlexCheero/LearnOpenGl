#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void SetupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex>& verts, std::vector<unsigned int>& inds, std::vector<Texture>& txts) : vertices(verts), indices(inds), textures(txts)
	{
		SetupMesh();
	}

	void Draw(Shader shader);
};

