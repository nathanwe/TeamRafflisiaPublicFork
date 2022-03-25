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
		normal(glm::vec3(0, 1, 0)),
		p1(glm::vec3(1, 0, 0)),
		p2(glm::vec3(0, 0, 1)),
		p3(glm::vec3(0, 0, 1)),
		p4(glm::vec3(0, 0, 1)),
		magnitude(1.0f), 
		minPoint(glm::vec3(0, 0, 0)), 
		maxPoint(glm::vec3(1, 1, 1)) 
	{}

	Shape shape;

	// In case of sphere
	float radius;

	// In case of Plane
	glm::vec3 normal;
	float magnitude;
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
	glm::vec3 p4;

	// In case of AABB
	glm::vec3 minPoint;
	glm::vec3 maxPoint;
};

inline void to_json(ordered_json& j, const Collider& collider) {
	to_json(j["Shape"], collider.shape);
	if (collider.shape == Shape::SPHERE || collider.shape == Shape::NIL)
	{
		to_json(j["Radius"], collider.radius);
	}	
	if (collider.shape == Shape::PLANE || collider.shape == Shape::NIL)
	{
		to_json(j["Normal"], collider.normal);
		to_json(j["Magnitude"], collider.magnitude);
	}
	if (collider.shape == Shape::AABB || collider.shape == Shape::NIL)
	{
		to_json(j["MinPoint"], collider.minPoint);
		to_json(j["MaxPoint"], collider.maxPoint);
	}
}

inline void from_json(const ordered_json& j, Collider& collider) {
	collider.shape = static_cast<Shape>(j["Shape"]);
	if (collider.shape == Shape::SPHERE || collider.shape == Shape::NIL)
	{
		from_json(j["Radius"], collider.radius);
	}
	if (collider.shape == Shape::PLANE || collider.shape == Shape::NIL)
	{
		from_json(j["Normal"], collider.normal);
		from_json(j["Magnitude"], collider.magnitude);
	}
	if (collider.shape == Shape::AABB || collider.shape == Shape::NIL)
	{
		from_json(j["MinPoint"], collider.minPoint);
		from_json(j["MaxPoint"], collider.maxPoint);
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