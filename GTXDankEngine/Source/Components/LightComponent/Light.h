#ifndef LIGHT_H
#define LIGHT_H
#include "pch.h"
#include "../utils/SerializationHelper.h"

enum LightType
{
	Directional,
	Point
};

class Light
{
public:
	glm::vec3 Color;
	glm::vec3 Intensity;

	// This memeber variable is for directional light only
	// The direction: Target - LightPosition
	// LightPosition is carried by Transform component
	glm::vec3 Target;

	LightType Type;

	Light();
	Light(LightType type, glm::vec3 c, glm::vec3 i);
	Light(LightType type, glm::vec3 c, glm::vec3 i, glm::vec3 t);


private:
	float Clip(float target, float max, float min);
};

inline void to_json(ordered_json& j, const Light& light) {
	to_json(j["Color"], light.Color);
	to_json(j["Intensity"], light.Intensity);
	to_json(j["LightType"], light.Type);
	to_json(j["Target"], light.Target);
}

inline void from_json(const ordered_json& j, Light& light) {
	from_json(j["Color"], light.Color);
	from_json(j["Intensity"], light.Intensity);
	light.Type = static_cast<LightType>(j["LightType"]);
	from_json(j["Target"], light.Target);
}

#endif