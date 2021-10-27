#ifndef COLLIDER_H
#define COLLIDER_H

#include "pch.h"
#include "../Core/Enums.h"

class Collider
{
public:
	Collider() {}

	Shape shape;

	// In case of sphere
	float radius;

	//// In case of Plane
	//glm::vec3 normal;
	//float magnitude;
};

#endif // !COLLIDER_H
