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

	void Delete(Entity e);
	void DeleteAll();

	int GetUsedMemory()
	{
		return memory.GetMemoryGotten();
	}

	inline T* GetComponentByEntity(Entity e);

	template <class U>
	std::set<Entity> Get2SharedEntitys(std::unordered_map<Entity, U*>& componentList1)
	{
		std::set<Entity> retval = std::set<Entity>();

		for (auto comp : componentList)
		{
			auto comp1 = componentList1.find(comp.first);
			if (comp1 != componentList1.end())
			{
				retval.insert(comp.first);
			}
		}
		return retval;
	}
	template <class U, class V>
	std::set<Entity> Get3SharedEntitys(std::unordered_map<Entity, U*>& componentList1, std::unordered_map<Entity, V*>& componentList2)
	{
		std::set<Entity> retval = std::set<Entity>();

		for (auto comp : componentList)
		{
			auto comp1 = componentList1.find(comp.first);
			if (comp1 != componentList1.end())
			{
				auto comp2 = componentList2.find(comp.first);
				if (comp2 != componentList2.end())
				{
					retval.insert(comp.first);
				}
			}
		}
		return retval;
	}
	template <class U, class V, class W>
	std::set<Entity> Get4SharedEntitys(std::unordered_map<Entity, U*>& componentList1, std::unordered_map<Entity, V*>& componentList2, std::unordered_map<Entity, W*>& componentList3)
	{
		std::set<Entity> retval = std::set<Entity>();

		for (auto comp : componentList)
		{
			auto comp1 = componentList1.find(comp.first);
			if (comp1 != componentList1.end())
			{
				auto comp2 = componentList2.find(comp.first);
				if (comp2 != componentList2.end())
				{
					auto comp3 = componentList3.find(comp.first);
					if (comp3 != componentList3.end())
					{
						retval.insert(comp.first);
					}
				}
			}
		}
		return retval;
	}
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
	if (EntityList.size() <= e || EntityList[e] == -1) // if entity does not exist
	{
		LOG_ERROR("Failed to add component, Entity {0} does not exist", e);
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
void AbstractComponentPool<T>::Delete(Entity e)
{
	auto search = componentList.find(e);
	if (search != componentList.end())
	{
		memory.FreeUsedBlock(search->second);
		componentList.erase(search);
	}
	else
	{
		LOG_INFO("Tried to delete component {0} of unkown Entity", typeid(T).name());
	}
}

template <class T>
void AbstractComponentPool<T>::DeleteAll()
{
	for (auto itr = componentList.begin(); itr != componentList.end(); itr++)
	{
		memory.FreeUsedBlock(itr->second);
	}
	componentList.clear();
}

template<class T>
inline T* AbstractComponentPool<T>::GetComponentByEntity(Entity e)
{
	auto comp = componentList.find(e);
	if (comp != componentList.end())
	{
		return comp->second;
	}
	//LOG_ERROR("component {0} not found", e);
	return nullptr;

}

#endif // !COMPONENTPOOL_H


