#include "pch.h"
#include "StillBodyComponent.h"

AbstractComponentPool<StillBodyComponent> StillBodyComponentPool;

StillBodyComponent::StillBodyComponent(Entity e)
	:AbstractComponent{ entity }
{
	LOG_INFO("Creating Still Body component to entity: {}", entity);
}


// Getters and Setters
glm::vec3 StillBodyComponent::getPostion()
{
	return this->position;
}
void StillBodyComponent::setPostion(glm::vec3 pos)
{
	this->position = pos;
}

std::pair<std::string, float> StillBodyComponent::getDebugDraw()
{
	std::string path = "Assets/models/Sphere/model.obj";
	float scale = this->BroadPhase.radius / 64;

	return std::pair<std::string, float>(path, scale);
}