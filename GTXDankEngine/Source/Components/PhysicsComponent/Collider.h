#ifndef COLLIDER_H
#define COLLIDER_H

#include "pch.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class Collider
{
public:

	Collider() : 
		shape(Shape::SPHERE), 
		radius(1.0f), 
		normal(glm::vec3(0, 0, 0)), 
		magnitude(1.0f), 
		minPoint(glm::vec3(0, 0, 0)), 
		max_point(glm::vec3(1, 1, 1)) 
	{}

	Shape shape;

	// In case of sphere
	float radius;

	// In case of Plane
	glm::vec3 normal;
	float magnitude;

	// In case of AABB
	glm::vec3 minPoint;
	glm::vec3 max_point;
};

inline void to_json(ordered_json& j, const Collider& collider) {
	to_json(j["Shape"], collider.shape);
	if (collider.shape == Shape::SPHERE)
	{
		to_json(j["Radius"], collider.radius);
	}	
	else if (collider.shape == Shape::PLANE)
	{
		to_json(j["Normal"], collider.normal);
		to_json(j["Magnitude"], collider.magnitude);
	}
	else if (collider.shape == Shape::AABB)
	{
		to_json(j["MinPoint"], collider.minPoint);
		to_json(j["MaxPoint"], collider.max_point);
	}
}

inline void from_json(const ordered_json& j, Collider& collider) {
	collider.shape = static_cast<Shape>(j["Shape"]);
	if (collider.shape == Shape::SPHERE)
		from_json(j["Radius"], collider.radius);
	else if (collider.shape == Shape::PLANE)
	{
		from_json(j["Normal"], collider.normal);
		from_json(j["Magnitude"], collider.magnitude);
	}
	else if (collider.shape == Shape::AABB)
	{
		from_json(j["MinPoint"], collider.minPoint);
		from_json(j["MaxPoint"], collider.max_point);
	}
}



#endif // !COLLIDER_H


//class SphericalCollider : public Collider
//{
//public:
//	float center;
//	float radius;
//};
//
//class PlanarCollider : public Collider
//{
//public:
//	float magnitude;
//	glm::vec3 center;
//	glm::vec3 normal;
//
//	// vertex points
//	std::vector<glm::vec3> vertisces;
//};
//
//
////
//class CubicalCollider : public Collider
//{
//public:
//	float center;
//	//float radius;
//};
//
//class CuboidalCollider : public Collider
//{
//public:
//	float center;
//	//float radius;
//};