#ifndef ENGINE_H
#define ENGINE_H

#include "pch.h"
#include "../Systems/GraphicsSystem/GraphicsSystem.h"
#include "../Systems/UISystem/UISystem.h"
#include "FramerateControlSystem/FramerateController.h"
#include "../Systems/InputSystem/InputManager.h"
#include "ResourceManager.h"
#include "../Systems/CommandSystem/CommandSystem.h"
#include "../Systems/AudioSystem/AudioSystem.h"
#include "../Systems/ScriptSystem/ScriptSystem.h"
#include "../Systems/EntitySystem/EntitySystem.h"
#include "../utils/common.h"
#include "GameObjectFactory.h"
#include "../utils/JsonFile.h"
#include "../Systems/SceneSystem/SceneSystem.h"
#include "../Systems/PhysicsSystem/PhysicsSystem.h"

extern std::string GAME_PATH;

extern ResourceManager<Model> ModelResourceManager;
extern ResourceManager<JsonFile> SerializationResourceManager;
extern ResourceManager<LuaFile> ScriptResourceManager;
extern ResourceManager<Texture> TextureResourceManger;


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

	float DeltaTime();

	std::shared_ptr<FramerateController> Framerate;

	// List all Systems
	// All systems will be allocated in stack
	// All systems will be init inside Engine::Init()
	// The lifecycle of all systems end when Engine instance get deleted 

	AudioSystem AudioSys;
	UISystem UISys;
	GraphicsSystem GraphicsSys;
	InputManager InputSys;
	CommandSystem CommandSys;
	EntitySystem EntitySys;
	SceneSystem SceneSys;
	GameObjectFactory GameObjectFac;
	ScriptSystem DoGameLogicScriptSys;
	ScriptSystem MenuSys;
	ScriptSystem FindGameSys;
	PhysicsSystem PhysicsSys;

	GLFWwindow* window;

	bool getMenuMode();
	void setMenuMode(bool mode);
	bool getDebugMode();
	void setDebugMode(bool mode);


private:
	bool menuMode = false;
	bool debugMode = false;

};
#endif // !ENGINE_H
