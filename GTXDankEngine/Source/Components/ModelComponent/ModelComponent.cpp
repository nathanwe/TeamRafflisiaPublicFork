#pragma once
#include "pch.h"
#include "../utils/Log.h"


#include "ModelComponent.h"

ComponentPool<ModelComponent> ModelComponentPool;

ModelComponent::ModelComponent(Entity entity, Model* m)
	:AbstractComponent{entity}, model(m)
{
	LOG_INFO("Creating model component to entity: {}", entity);
}
