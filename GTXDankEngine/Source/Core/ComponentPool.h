
#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#include "pch.h"
#include "Engine.h"




// Abstract base class for all component managers
template <typename T>
class ComponentPool
{
public:
	virtual ~ComponentPool() {}

	// new componet will overwrite the old one

	bool Add(Entity e, T component);
	bool Delete(Entity e);


	// AbstractComponent pointer instead or handle instead?
	// Map Entity and component handle
	std::unordered_map<Entity, T> ComponentList;

};
#endif // !COMPONENTPOOL_H