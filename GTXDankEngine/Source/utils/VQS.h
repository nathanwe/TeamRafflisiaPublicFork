#ifndef VQS_H
#define VQS_H

#include "pch.h"
#include "../utils/SerializationHelper.h"

class VQS
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;


	// Default constructor
	// position: origin, scale: 1.0, rotation: zero rotation
	VQS(void) : position(glm::vec3(0.0)), scale(glm::vec3(1.0)), rotation(glm::quat(1.0, 0.0, 0.0, 0.0)) {}

	// quaternion get normalized automatically
	VQS(glm::vec3 v, glm::quat q, glm::vec3 s) : position(v), scale(s), rotation(glm::normalize(q)) {}

	// quaternion will be set to zero rotation
	VQS(glm::vec3 v, glm::vec3 s) : position(v), scale(s), rotation(glm::quat(1.0, 0.0, 0.0, 0.0)) {}


	glm::mat4 Matrix(void) const;
};

inline void to_json(ordered_json& j, const VQS& vqs) {
	to_json(j["position"], vqs.position);
	to_json(j["scale"], vqs.scale);
	to_json(j["rotation"], vqs.rotation);
}

inline void from_json(const ordered_json& j, VQS& vqs) {
	from_json(j["position"], vqs.position);
	from_json(j["scale"], vqs.scale);
	from_json(j["rotation"], vqs.rotation);
}

#endif // !VQS_H
