#ifndef VQS_H
#define VQS_H

#include "pch.h"


class VQS
{
public:
	glm::vec3 position;
	float scale;
	glm::quat rotation;


	// Default constructor
	// position: origin, scale: 1.0, rotation: zero rotation
	VQS(void) : position(glm::vec3(0.0)), scale(1.0), rotation(glm::quat(1.0, 0.0, 0.0, 0.0)) {}

	// quaternion get normalized automatically
	VQS(glm::vec3 v, glm::quat q, float s) : position(v), scale(s), rotation(glm::normalize(q)) {}

	// quaternion will be set to zero rotation
	VQS(glm::vec3 v, float s) : position(v), scale(s), rotation(glm::quat(1.0, 0.0, 0.0, 0.0)) {}


	glm::mat4 Matrix(void) const;
};
#endif // !VQS_H
