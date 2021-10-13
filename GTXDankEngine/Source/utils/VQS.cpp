#include "pch.h"

#include "VQS.h"


glm::mat4 VQS::Matrix(void) const
{
	glm::mat4 t = glm::translate(glm::mat4(1.0f), position);

	// convert rotation quaternion to matrix
	glm::mat4 r = glm::toMat4(this->rotation);	

	// always uniform scale
	glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale));

	// this multiplication order matters
	return t * r * s;
}
