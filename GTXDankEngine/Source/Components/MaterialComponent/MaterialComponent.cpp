
#include "pch.h"

#include "../utils/Log.h"
#include "MaterialComponent.h"

AbstractComponentPool<MaterialComponent> MaterialComponentPool;


MaterialComponent::MaterialComponent(Entity entity, Material* mat)
	:AbstractComponent{ entity }, material(mat)
{
	LOG_INFO("Creating Material component to entity: {}", entity);
}

