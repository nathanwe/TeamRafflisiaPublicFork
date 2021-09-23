#ifndef VQS_H
#define VQS_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct VQS
{
	// quaternion get normalized automatically
	VQS(glm::vec3 t, float s, glm::quat r) : translation(t), scale(s), rotation(glm::normalize(r)) {}

	glm::vec3 translation;
	float scale;
	glm::quat rotation;
};
#endif // !VQS_H
