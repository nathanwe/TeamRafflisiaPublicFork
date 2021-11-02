#ifndef COLLIDER_H
#define COLLIDER_H

#include "pch.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class Collider
{
public:

	Shape shape;

	// In case of sphere
	float radius;

	//// In case of Plane
	//glm::vec3 normal;
	//float magnitude;
};

inline void to_json(ordered_json& j, const Collider& collider) {
	to_json(j["Shape"], collider.shape);
	to_json(j["Radius"], collider.radius);
}

inline void from_json(const ordered_json& j, Collider& collider) {
	collider.shape = static_cast<Shape>(j["Shape"]);
	from_json(j["Radius"], collider.radius);
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