#ifndef MOVING_BODY_COMPONENT_H
#define MOVING_BODY_COMPONENT_H

#include "pch.h"
//#include "../Core/Enums.h"
#include "../Core/ComponentPool.h"
#include "../Components/PhysicsComponent/Collider.h"
#include "RigidBody.h"

class MovingBodyComponent : public AbstractComponent
{
public:
	MovingBodyComponent(Entity e);

public:
	RigidBody rigidBody;

	Collider BroadPhase;
	std::vector<Collider> NarrowPhase;

};

extern AbstractComponentPool<MovingBodyComponent> MovingBodyComponentPool;


#endif // !MOVING_BODY_COMPONENT_H
