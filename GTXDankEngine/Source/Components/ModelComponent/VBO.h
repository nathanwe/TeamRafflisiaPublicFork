#ifndef VBO_H
#define VBO_H



#include "pch.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texUV;
};

class VBO {
	public:
		GLuint ID;
		VBO(std::vector<Vertex>& vertices);

		void Bind();
		void Unbind();
		void Delete();
};
#endif // !VBO_H
