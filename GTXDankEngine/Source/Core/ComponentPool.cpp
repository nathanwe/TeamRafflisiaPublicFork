
#pragma once
#include "pch.h"
#include "../utils/Log.h"
#include "ComponentPool.h"
#include "Engine.h"

template <typename T>
bool ComponentPool<T>::Add(Entity e, T component)
{
	if (EntityList[e] == 0) // if entity does not exist
		LOG_ERROR("Failed to add component, Entity does not exist");

	ComponentList.insert(std::make_pair(e, component));
	LOG_INFO("Add component to Entity {}", e);
	return true;
}

template <typename T>
bool ComponentPool<T>::Delete(Entity e)
{

}
