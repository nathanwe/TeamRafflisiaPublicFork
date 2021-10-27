#include "pch.h"
#include "MovingBodyComponent.h"

AbstractComponentPool<MovingBodyComponent> MovingBodyComponentPool;

MovingBodyComponent::MovingBodyComponent(Entity e)
	:AbstractComponent{ entity }
{
	LOG_INFO("Creating Moving Body component to entity: {}", entity);
}