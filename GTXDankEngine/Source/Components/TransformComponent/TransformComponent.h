#pragma once

#include "Engine.h"
#include "VQS.h"

class TransformComponent : public Component
{
public:
	TransformComponent(Entity e, VQS transform) : Component(e), transform(transform) {}
	~TransformComponent() {}

	VQS transform;	
};



class TransformComponentMgr : public ComponentManager<TransformComponent>
{
public:
	// Add()
	// Delete()
	// Get();
};

