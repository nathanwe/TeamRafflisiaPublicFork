#pragma once

#include "pch.h"
#include "../../Core/System.h"
#include "../../utils/common.h"

class EntitySystem : public System
{
public:

	EntitySystem() = default;
	~EntitySystem() = default;

	bool Init() override;
	void Update(float timeStamp = 0);
	bool Destroy() override;

	Entity CreateEntity();
	//void DestroyEntity(Entity e);
	//void SetArchetype();
	//Archetype GetArchetype();
	//bool HasComponent
	//EnqueueDestroyEntity
	//update

private:
	std::set<Entity> availableEntities;
	std::set<Entity> allocatedEntities;
	int entityCount;

};