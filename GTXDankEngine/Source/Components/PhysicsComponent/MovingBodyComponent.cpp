#include "pch.h"
#include "MovingBodyComponent.h"

AbstractComponentPool<MovingBodyComponent> MovingBodyComponentPool;

MovingBodyComponent::MovingBodyComponent(Entity e, RigidBody rigidBody)
	:AbstractComponent{ entity }, rigidBody(rigidBody)
{
	LOG_INFO("Creating Moving Body component to entity: {}", entity);
}