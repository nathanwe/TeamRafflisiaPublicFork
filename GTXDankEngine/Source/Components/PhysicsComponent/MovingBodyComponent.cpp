#include "pch.h"
#include "MovingBodyComponent.h"

AbstractComponentPool<MovingBodyComponent> MovingBodyComponentPool;

MovingBodyComponent::MovingBodyComponent(Entity e, RigidBody rigidBody)
	:AbstractComponent(e), rigidBody(rigidBody)
{
	LOG_INFO("Creating Moving Body component to entity: {}", e);
}


// Getters and Setters

std::pair<std::string, float> MovingBodyComponent::getDebugDraw()
{
	std::string path = GAME_PATH + std::string("Assets/models/Sphere/model.obj");
	float scale = this->BroadPhase.radius / 64;

	return std::pair<std::string, float>(path, scale);
}

glm::vec3 MovingBodyComponent::getPostion()
{
	return this->rigidBody.position;
}
void MovingBodyComponent::setPostion(glm::vec3 pos)
{
	this->rigidBody.position = pos;
}

glm::vec3 MovingBodyComponent::getVelocity()
{
	return this->rigidBody.velocity;
}
void MovingBodyComponent::setVelocity(glm::vec3 vel)
{
	this->rigidBody.velocity = vel;
}

glm::vec3 MovingBodyComponent::getAccelration()
{
	return this->rigidBody.acceleration;
}
void MovingBodyComponent::setAcceleration(glm::vec3 acc)
{
	this->rigidBody.acceleration = acc;
}

glm::quat MovingBodyComponent::getOrientation()
{
	return this->rigidBody.orientation;
}
void MovingBodyComponent::setOrientation(glm::quat orient)
{
	this->rigidBody.orientation = orient;
}

glm::quat MovingBodyComponent::getAngularVelocity()
{
	return this->rigidBody.angularVelocity;
}
void MovingBodyComponent::setAngularVelocity(glm::quat omega)
{
	this->rigidBody.angularVelocity = omega;
}

glm::quat MovingBodyComponent::getTorque()
{
	return this->rigidBody.torque;
}
void MovingBodyComponent::setTorque(glm::quat tor)
{
	this->rigidBody.torque = tor;
}

bool MovingBodyComponent::getGravity()
{
	return this->rigidBody.isGravity;
}
void MovingBodyComponent::setGravity(bool gravity)
{
	this->rigidBody.isGravity = gravity;
}

float MovingBodyComponent::getElasticity()
{
	return this->rigidBody.elasticity;
}
void MovingBodyComponent::setElasticity(float elasticity)
{
	this->rigidBody.elasticity = elasticity;
}