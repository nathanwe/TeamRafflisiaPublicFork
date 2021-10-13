#include "pch.h"
#include "EntitySystem.h"
#include "../../utils/common.h"
#include "../../utils/Log.h"
#include "../../Core/System.h"
#include "../../Core/Engine.h"

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
}

bool EntitySystem::Destroy()
{
    return false;
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
