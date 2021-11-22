#include "pch.h"
#include "ColliderComponent.h"

AbstractComponentPool<ColliderComponent> ColliderComponentPool;

ColliderComponent::ColliderComponent(Entity e)
	:AbstractComponent(e)
{
	LOG_INFO("Creating Collider component to entity: {}", e);
}

ColliderComponent::ColliderComponent(Entity e, Collider narrowPhase)
	: NarrowPhase(narrowPhase), AbstractComponent(e)
{
	LOG_INFO("Creating Collider component to entity: {}", e);
}


// Getters and Setters
glm::vec3 ColliderComponent::getPostion()
{
	return this->position;
}
void ColliderComponent::setPostion(glm::vec3 pos)
{
	this->position = pos;
}

std::pair<std::string, float> ColliderComponent::getDebugDraw()
{
	std::string path = GAME_PATH + std::string("Assets/models/Sphere/model.obj");
	float scale = this->NarrowPhase.radius / 64;

	return std::pair<std::string, float>(path, scale);
}