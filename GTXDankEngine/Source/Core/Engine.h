#pragma once
#include "pch.h"
#include "../Systems/GraphicsSystem/GraphicsSystem.h"

typedef uint32_t Entity;

extern std::vector<Entity> EntityList;

// Abstract base class for all components
class AbstractComponent
{
public:

	// An entity must be passed when creating a new component
	AbstractComponent(Entity e) : entity(e) {}
	virtual ~AbstractComponent() {}

	inline Entity GetEntity() const { return entity; }

	// current component belongs to this entity
	Entity entity;

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

