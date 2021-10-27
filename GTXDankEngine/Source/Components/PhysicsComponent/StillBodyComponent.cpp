#include "pch.h"
#include "StillBodyComponent.h"

AbstractComponentPool<StillBodyComponent> StillBodyComponentPool;

StillBodyComponent::StillBodyComponent(Entity e)
	:AbstractComponent{ entity }
{
	LOG_INFO("Creating Still Body component to entity: {}", entity);
}