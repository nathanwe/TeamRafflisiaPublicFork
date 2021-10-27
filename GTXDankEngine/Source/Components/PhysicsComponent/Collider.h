#ifndef COLLIDER_H
#define COLLIDER_H

#include "pch.h"
#include "../Core/Enums.h"

class Collider
{
public:

	Shape shape = Shape::SPHERE;

	// In case of sphere
	float radius = 1.0f;

	//// In case of Plane
	//glm::vec3 normal;
	//float magnitude;
};

#endif // !COLLIDER_H
