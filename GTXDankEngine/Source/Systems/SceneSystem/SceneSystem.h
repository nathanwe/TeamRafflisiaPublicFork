#pragma once

#include "pch.h"
#include "../utils/common.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class SceneSystem
{
public:
	bool Init();

	void UnloadScene();
	void LoadScene(int level);
	void LoadNextLevel();
	void LoadPreviousLevel();

private:
	ordered_json levels;
	int currentLevel = 0;
};

