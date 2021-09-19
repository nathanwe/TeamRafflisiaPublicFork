#include "pch.h"
#include "VBO.h"

// VBO constructor
VBO::VBO(std::vector<Vertex>& vertices) 
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Bind VBO
void VBO::Bind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbind VBO
void VBO::Unbind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Delete VBO
void VBO::Delete() 
{
	glDeleteBuffers(1, &ID);
}