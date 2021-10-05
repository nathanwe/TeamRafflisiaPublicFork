#pragma once

#include "pch.h"
#include "../Core/ComponentPool.h"
// #include "../Core/ResourceManager.h"

#include "RoutineList.h"

class RoutineComponent : public AbstractComponent
{
public:
	RoutineComponent(Entity entity, RoutineList* rr);

	RoutineList* list;
};

extern AbstractComponentPool<RoutineComponent> RoutineComponentPool;
// extern ResourceManager<Model> ModelResourceManager;
