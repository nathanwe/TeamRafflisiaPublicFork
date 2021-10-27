#ifndef STILL_BODY_COMPONENT_H
#define STILL_BODY_COMPONENT_H

#include "pch.h"
#include "../Core/Enums.h"
#include "../Core/ComponentPool.h"
#include "Collider.h"
#include <vector>

class StillBodyComponent : public AbstractComponent
{
public:
	StillBodyComponent(Entity e);

public:
	glm::vec3 position;
	Collider BroadPhase;
	std::vector<Collider> NarrowPhase;

};

extern AbstractComponentPool<StillBodyComponent> StillBodyComponentPool;






#endif // !STILL_BODY_COMPONENT_H
