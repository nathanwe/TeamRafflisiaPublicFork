#pragma once
#include "pch.h"

#include "Shader.h"



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	//	glm::vec3 Normal;
};



class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices);

	void Draw(Shader& shader) const;
	void setup();

private:
	unsigned int VAO;

};

