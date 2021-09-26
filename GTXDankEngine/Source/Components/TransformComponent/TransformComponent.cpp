#include "pch.h"

#include "../utils/Log.h"
#include "TransformComponent.h"

AbstractComponentPool<TransformComponent> TransformComponentPool;

TransformComponent::TransformComponent(Entity entity, VQS* vqs)
	:AbstractComponent{ entity }, transform(vqs)
{
	LOG_INFO("Creating transform component to entity: {}", entity);
}
