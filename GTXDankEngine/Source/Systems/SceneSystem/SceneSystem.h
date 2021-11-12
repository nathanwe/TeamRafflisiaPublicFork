#ifndef SCENESYSTEM_H
#define SCENESYSTEM_H



#include "pch.h"
#include "../Core/System.h"
#include "../utils/common.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class SceneSystem : public System
{
public:
	bool Init();

	void UnloadScene();
	void LoadScene(int level);
	void LoadCurrentLevel();
	void LoadNextLevel();
	void LoadPreviousLevel();
	void SaveCurrentLevel();
	void SaveAsNewLevel();
	void Update(float dt);
	bool Destroy();
	int GetCurrentLevel();

private:
	
	ordered_json levels;
	int currentLevel = 0;
	int levelToLoad = 0;
	bool shouldLoadLevel = false;
};
#endif // !SCENESYSTEM_H
