#ifndef STILL_BODY_COMPONENT_H
#define STILL_BODY_COMPONENT_H

#include "pch.h"
#include "../Core/Enums.h"
#include "../Core/ComponentPool.h"
#include "Collider.h"

class StillBodyComponent : public AbstractComponent
{
public:
	StillBodyComponent(Entity e);

public:
	glm::vec3 position;
	Collider BroadPhase;
	//std::vector<Collider> NarrowPhase;


	// Getters and Setters
	glm::vec3 getPostion();
	void setPostion(glm::vec3 pos);

	std::pair<std::string, float> getDebugDraw();

};

extern AbstractComponentPool<StillBodyComponent> StillBodyComponentPool;






#endif // !STILL_BODY_COMPONENT_H
