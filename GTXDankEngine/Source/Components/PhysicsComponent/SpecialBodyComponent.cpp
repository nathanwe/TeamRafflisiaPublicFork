#include "pch.h"
#include "SpecialBodyComponent.h"

AbstractComponentPool<SpecialBodyComponent> SpecialBodyComponentPool;

SpecialBodyComponent::SpecialBodyComponent(Entity e, RigidBody rigidBody)
	:AbstractComponent{ entity }, rigidBody(rigidBody)
{
	LOG_INFO("Creating Moving Body component to entity: {}", entity);
}


// Getters and Setters

std::pair<std::string, float> SpecialBodyComponent::getDebugDraw()
{
	std::string path = GAME_PATH + std::string("Assets/models/Sphere/model.obj");
	float scale = this->BroadPhase.radius / 64;

	return std::pair<std::string, float>(path, scale);
}

glm::vec3 SpecialBodyComponent::getPostion()
{
	return this->rigidBody.position;
}
void SpecialBodyComponent::setPostion(glm::vec3 pos)
{
	this->rigidBody.position = pos;
}

glm::vec3 SpecialBodyComponent::getVelocity()
{
	return this->rigidBody.velocity;
}
void SpecialBodyComponent::setVelocity(glm::vec3 vel)
{
	this->rigidBody.velocity = vel;
}

glm::vec3 SpecialBodyComponent::getAccelration()
{
	return this->rigidBody.acceleration;
}
void SpecialBodyComponent::setAcceleration(glm::vec3 acc)
{
	this->rigidBody.acceleration = acc;
}

glm::quat SpecialBodyComponent::getOrientation()
{
	return this->rigidBody.orientation;
}
void SpecialBodyComponent::setOrientation(glm::quat orient)
{
	this->rigidBody.orientation = orient;
}

glm::quat SpecialBodyComponent::getAngularVelocity()
{
	return this->rigidBody.angularVelocity;
}
void SpecialBodyComponent::setAngularVelocity(glm::quat omega)
{
	this->rigidBody.angularVelocity = omega;
}

glm::quat SpecialBodyComponent::getTorque()
{
	return this->rigidBody.torque;
}
void SpecialBodyComponent::setTorque(glm::quat tor)
{
	this->rigidBody.torque = tor;
}

bool SpecialBodyComponent::getGravity()
{
	return this->rigidBody.isGravity;
}
void SpecialBodyComponent::setGravity(bool gravity)
{
	this->rigidBody.isGravity = gravity;
}

float SpecialBodyComponent::getElasticity()
{
	return this->rigidBody.elasticity;
}
void SpecialBodyComponent::setElasticity(float elasticity)
{
	this->rigidBody.elasticity = elasticity;
}