#ifndef MODEL_H
#define MODEL_H



#include "pch.h"
#include "Mesh.h"

/*
* .obj model file
* Architecture of Model:
* Each model contains a tree of nodes
* Each node contains meshes
* Each mesh contain vertices, indices, Material(textures)
*/


class Model
{
public:
	Model(const char* path);
	Model(std::string path);
	void Draw(Shader& shader) const;
	void OnLoad();

private:
	std::vector<Mesh> meshes;

	// path to current model object
	std::string directory;


	// Entry point of loading a model
	void loadModel(std::string path);

	// loading a tree of nodes recursives, for each node, load its mesh
	void processNode(aiNode* node, const aiScene* scene);

	// load vertices, indices and material(textures) from mesh
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Vertex> processVertices(aiMesh* mesh) const;
	std::vector<unsigned int> processIndices(aiMesh* mesh) const;

	// helper function, convert aiVector3D to glm::vec3
	void setVec3(glm::vec3& des, aiVector3D& source) const;

	//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
#endif // !MODEL_H
