#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "pch.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class RigidBody
{
public:

	//Properties
	float elasticity;
	float friction;
	bool isGravity;
	float mass;
	CollisionType collisionType;

	//State
	bool isColliding;

	// Current Frame
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	// Previous Frame
	glm::vec3 prevPosition;
	glm::vec3 prevVelocity;
	glm::vec3 prevAcceleration;
};

#endif // !RIGID_BODY_H

inline void to_json(ordered_json& j, const RigidBody& rigidBody) {
	to_json(j["Elasticity"], rigidBody.elasticity);
	to_json(j["Friction"], rigidBody.friction);
	to_json(j["Gravity"], rigidBody.isGravity);
	to_json(j["Position"], rigidBody.position);
	to_json(j["Velocity"], rigidBody.velocity);
	to_json(j["Accelereation"], rigidBody.acceleration);
	//to_json(j["CollisionType"], rigidBody.collisionType);
}

inline void from_json(const ordered_json& j, RigidBody& rigidBody) {
	from_json(j["Elasticity"], rigidBody.elasticity);
	from_json(j["Friction"], rigidBody.friction);
	from_json(j["Gravity"], rigidBody.isGravity);
	from_json(j["Position"], rigidBody.position);
	from_json(j["Velocity"], rigidBody.velocity);
	from_json(j["Accelereation"], rigidBody.acceleration);
}