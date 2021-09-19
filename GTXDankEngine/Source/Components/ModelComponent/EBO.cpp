#include "pch.h"
#include "EBO.h"

// EBO constructor
EBO::EBO(std::vector<GLuint>& indices) 
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Bind EBO
void EBO::Bind() 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbind EBO
void EBO::Unbind() 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Delete EBO
void EBO::Delete() 
{
	glDeleteBuffers(1, &ID);
}