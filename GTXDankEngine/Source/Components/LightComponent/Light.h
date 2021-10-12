#ifndef LIGHT_H
#define LIGHT_H
#include "pch.h"

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

	Light(LightType type, glm::vec3 c, glm::vec3 i);
	Light(LightType type, glm::vec3 c, glm::vec3 i, glm::vec3 t);


private:
	float Clip(float target, float max, float min);
};

#endif