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

	LightType Type;

	Light(LightType type, glm::vec3 c, glm::vec3 i);


private:
	float Clip(float target, float max, float min);
};

#endif