#include "pch.h"
#include "EntitySystem.h"
#include "../../utils/common.h"
#include "../../utils/Log.h"
#include "../../Core/System.h"
#include "../../Core/Engine.h"

#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/TagComponent/TagComponent.h"


#include "../ProfileSystem/ProfileSystem.h"

extern Engine engine;

bool EntitySystem::Init()
{
    for (Entity id = 0; id < MAX_ENTITIES; ++id)
    {
        availableEntities.insert(id);
    }
    return true;
}

void EntitySystem::Update(float timeStamp)
{
	Timer timer("Entity Update");

	if (deleteAllQueue)
	{

		DeleteAllQueuedEntities();
		deleteAllQueue = false;
	}
	else
	{
		for (Entity e : deleteQueue)
		{
			DestroyQueuedEntity(e);
		}
		deleteQueue.clear();
	}

}

bool EntitySystem::Destroy()
{
    return true;
}

Entity EntitySystem::CreateEntity()
{
	assert(entityCount < MAX_ENTITIES && "Entity Limit Exceeded");
	Entity id = *availableEntities.begin();
	allocatedEntities.insert(id);
	availableEntities.erase(availableEntities.begin());
	++entityCount;
	LOG_TRACE("[Entity ID: {}] Entity created", id);
	return id;
}

void EntitySystem::DestroyEntity(Entity e)
{
	deleteQueue.insert(e);
}

void EntitySystem::DeleteAllEntities()
{
	deleteAllQueue = true;
}

void EntitySystem::DestroyQueuedEntity(Entity e)
{
	if (allocatedEntities.find(e) == allocatedEntities.end())
	{
		LOG_TRACE("[Entity ID: {}] Entity not destroyed", e);
		return;
	}

	allocatedEntities.erase(e);
	availableEntities.insert(e);
	--entityCount;
	LOG_TRACE("[Entity ID: {}] Entity destroyed", e);
	TransformComponentPool.Delete(e);
	GameLogicCategoryComponentPool.Delete(e);
	MaterialComponentPool.Delete(e);
	ModelComponentPool.Delete(e);
	LightComponentPool.Delete(e);
	StillBodyComponentPool.Delete(e);
	MovingBodyComponentPool.Delete(e);
	TagComponentPool.Delete(e);
}

void EntitySystem::DeleteAllQueuedEntities()
{
	GameLogicCategoryComponentPool.DeleteAll();
	ModelComponentPool.DeleteAll();
	MaterialComponentPool.DeleteAll();
	LightComponentPool.DeleteAll();
	//
	StillBodyComponentPool.DeleteAll();
	MovingBodyComponentPool.DeleteAll();
	//
	GameLogicCategoryComponentPool.DeleteAll();
	TagComponentPool.DeleteAll();
	TransformComponentPool.DeleteAll();
	for (Entity e : allocatedEntities)
	{
		availableEntities.insert(e);
	}
	allocatedEntities.clear();
	entityCount = 0;
}
