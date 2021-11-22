#ifndef SPECIAL_BODY_COMPONENT_H
#define SPECIAL_BODY_COMPONENT_H

#include "pch.h"
//#include "../Core/Enums.h"
#include "../Core/ComponentPool.h"
#include "../Components/PhysicsComponent/Collider.h"
#include "RigidBody.h"

class SpecialBodyComponent : public AbstractComponent
{
public:
	SpecialBodyComponent(Entity e, RigidBody rigidBody);

public:
	RigidBody rigidBody;

	Collider BroadPhase;
	//std::vector<Collider> NarrowPhase;

	// Getters and Setters
	std::pair<std::string, float> getDebugDraw();

	glm::vec3 getPostion();
	void setPostion(glm::vec3 pos);

	glm::vec3 getVelocity();
	void setVelocity(glm::vec3 vel);

	glm::vec3 getAccelration();
	void setAcceleration(glm::vec3 acc);

	glm::quat getOrientation();
	void setOrientation(glm::quat orient);

	glm::quat getAngularVelocity();
	void setAngularVelocity(glm::quat omega);

	glm::quat getTorque();
	void setTorque(glm::quat tor);

	bool getGravity();
	void setGravity(bool gravity);

	float getElasticity();
	void setElasticity(float elasticity);


};

inline void to_json(ordered_json& j, const SpecialBodyComponent& component) 
{
	to_json(j, component.rigidBody);
	to_json(j, component.BroadPhase);
}

extern AbstractComponentPool<SpecialBodyComponent> SpecialBodyComponentPool;


#endif // !SPECIAL_BODY_COMPONENT_H
