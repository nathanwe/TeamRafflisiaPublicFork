#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#include "pch.h"
#include "Engine.h"
#include "MemoryPool.h"

const unsigned int PAGE_SIZE = 4096;



// Abstract base class for all component managers
template <class T>
class AbstractComponentPool
{
public:
	AbstractComponentPool();
	virtual ~AbstractComponentPool() {}

	// new componet will overwrite the old one
	template<typename... constructorArguments>
	bool Add(Entity e, constructorArguments... args);

	bool Delete(Entity e);


	// AbstractComponent pointer instead or handle instead?
	// Map Entity and component handle
	std::unordered_map<Entity, T*> componentList;
private:
	MemoryPool<sizeof(T), PAGE_SIZE / sizeof(T)> memory;

};

//.cpp stuff in .h for template reasons
template<class T>
template<typename ...constructorArguments>
bool AbstractComponentPool<T>::Add(Entity e, constructorArguments... args)
{
	if (EntityList[e] == 0) // if entity does not exist
	{
		LOG_ERROR("Failed to add component, Entity does not exist");
	}

	void* location = memory.GetFreeBlock();
	T* component = new (location) T(e, args...);
	componentList.insert(std::make_pair(e, component));
	LOG_INFO("Add component to Entity {}", e);
	return true;
}

template<class T>
AbstractComponentPool<T>::AbstractComponentPool()
{
	static_assert(std::is_base_of<AbstractComponent, T>::value, "Tried to make a ComponentPool of not a Component");
}

template <class T>
bool AbstractComponentPool<T>::Delete(Entity e)
{
	//TODO
}

#endif // !COMPONENTPOOL_H


