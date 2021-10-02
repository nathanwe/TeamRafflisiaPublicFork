#ifndef QUAD_H
#define QUAD_H

#include "pch.h"

class Shader;


class Quad
{
public:
	Quad();
	~Quad();

	// delete copy and assignment constructor
	Quad(const Quad&) = delete;
	Quad& operator= (const Quad&) = delete;

	void Draw(Shader& shader) const;

private:
	unsigned int VAO;
	unsigned int quadVBO;

};


#endif