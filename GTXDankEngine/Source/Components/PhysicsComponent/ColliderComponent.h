#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "pch.h"
#include "../Core/Enums.h"
#include "../Core/ComponentPool.h"
#include "Collider.h"


class ColliderComponent : public AbstractComponent
{
public:
	ColliderComponent(Entity e);
	ColliderComponent(Entity e, Collider narrowPhase);

public:
	glm::vec3 position;
	std::pair<int, int> BroadPhase;
	Collider NarrowPhase;


	// Getters and Setters
	glm::vec3 getPostion();
	void setPostion(glm::vec3 pos);

	std::pair<std::string, float> getDebugDraw();

};


extern AbstractComponentPool<ColliderComponent> ColliderComponentPool;


#endif // !STILL_BODY_COMPONENT_H
