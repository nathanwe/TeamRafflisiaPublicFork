#include "pch.h"

#ifndef COMPONENTPOOL2_H
#define COMPONENTPOOL2_H


#include "Enums.h"
#include "MemoryPool.h"
#include "Engine.h"
#include <map>



const unsigned int PAGE_SIZE = 4096;
typedef uint32_t Entity;

template<class Component>
class ComponentPool// : System
{
public:
	ComponentPool();
	void addComponent(Component* componentToAdd);
	bool Init();
	void Update(std::chrono::milliseconds timeStamp = 0);
	bool Destory();

	std::map<Entity, Component*> components
};
//.cpp stuff in header for template reasons.
template<class Component>
inline ComponentPool<Component>::ComponentPool()
{
	static_assert(std::is_base_of<AbstractComponent, Component>::value, "Tried to make a ComponentPool of not a Component");
}

template<class Component>
inline void ComponentPool<Component>::addComponent(Component* componentToAdd)
{
	components.push_back(std::make_pair< Entity, Component>(componentToAdd.getEntity(), componentToAdd);
}

template<class Component>
inline bool ComponentPool<Component>::Init()
{
	return true;
}

template<class Component>
inline void ComponentPool<Component>::Update(std::chrono::milliseconds timeStamp)
{
	for (auto &comp : components)
	{
		comp.second()->Update(timeStamp);
	}
}

template<class Component>
inline bool ComponentPool<Component>::Destory()
{
	return false;
}

#endif // !COMPONENTPOOL2_H
