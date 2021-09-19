#pragma once
#include "pch.h"
#include "../Systems/GraphicsSystem/GraphicsSystem.h"

typedef uint32_t Entity;



// Abstract base class for all components
class Component
{
public:

	// An entity must be passed when creating a new component
	Component(Entity e) : entity(e) {}
	virtual ~Component() {}

	inline Entity GetEntity() const { return entity; }

	// current component belongs to this entity
	Entity entity;

};


// Abstract base class for all component managers
template <class T>
class ComponentManager
{
public:
	virtual ~ComponentManager() {}

protected:
	// Component pointer instead or handle instead?
	// Map Entity and component handle
	std::unordered_map<Entity, T> ComponentList;

};

class Engine
{
public:

	// default constructor
	Engine() = default;

	// delete copy and assignment constructor
	Engine(const Engine&) = delete;
	Engine& operator= (const Engine&) = delete;


	bool Init();

	void Run();

	void Destroy();


private:
	// List all Systems
	// All systems will be allocated in stack
	// All systems will be init inside Engine::Init()
	// The lifecycle of all systems end when Engine instance get deleted 

	/*
	MemorySystem MemorySys;

	ResourceSystem ResourceSys;

	EntitySystem EntitySys;

	FrameRateSystem FrameRateSys;

	.......

	PhysicsSystem PhysicsSys;

	GraphicsSystem GraphicsSys;
	*/

	GraphicsSystem GraphicsSys;
};

