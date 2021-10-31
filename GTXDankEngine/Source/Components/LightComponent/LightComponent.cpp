#include "pch.h"
#include "LightComponent.h"

AbstractComponentPool<LightComponent> LightComponentPool;

LightComponent::LightComponent(Entity e, Light light)
	:AbstractComponent{ entity }, LightSource(light)
{
	LOG_INFO("Creating Light component to entity: {}", entity);
}