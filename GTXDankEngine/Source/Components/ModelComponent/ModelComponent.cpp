#include "pch.h"
#include "../utils/Log.h"

#include "ModelComponent.h"
#include "../Core/ResourceManager.h"

AbstractComponentPool<ModelComponent> ModelComponentPool;

ModelComponent::ModelComponent(Entity entity, ResourceHandle<Model>* m)
	:AbstractComponent{entity}, model(m)
{
	LOG_INFO("Creating model component to entity: {}", entity);
}
