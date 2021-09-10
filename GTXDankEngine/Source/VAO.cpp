#include "VAO.h"

// VAO constructor
VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

// Binds a given VBO for the VAO to collect info from
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Bind VAO
void VAO::Bind() {
	glBindVertexArray(ID);
}

// Unbind VAO
void VAO::Unbind() {
	glBindVertexArray(0);
}

// Delete VAO
void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}