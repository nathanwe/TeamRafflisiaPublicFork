#include "pch.h"
#include "RoutineComponent.h"

AbstractComponentPool<RoutineComponent> RoutineComponentPool;

RoutineComponent::RoutineComponent(Entity entity, RoutineList* rList)
	:AbstractComponent{ entity }, list(rList)
{
	LOG_INFO("Creating Routine component to entity: {}", entity);
}
