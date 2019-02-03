#pragma once
#include <vector>
#include <string>

#include <assimp/scene.h>

#include "Mesh.h"

class Model
{
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh> meshes;
	std::string directory;
	
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model(std::string path) { LoadModel(path); }
	void Draw(Shader shader);
};

