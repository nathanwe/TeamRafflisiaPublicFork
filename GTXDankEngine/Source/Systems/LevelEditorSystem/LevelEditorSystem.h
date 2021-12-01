#ifndef LEVEL_EDITOR_SYSTEM_H
#define LEVEL_EDITOR_SYSTEM_H

#include "pch.h"
#include "../../Core/System.h"

class LevelEditorSystem : public System
{
public:
	LevelEditorSystem();
	bool Init();
	void Update(float timeStamp);
	bool Destroy();
	void UpdateGUI();
	int entityID;
	int gameLogicID;

private:
	int prevEntityID;
	// std::string entityCategory;
	// int entityCategoryIndex = -1;

	void ChangeSelectedObject();
	std::string entityCategory;
	int entityCategoryIndex = -1;
	glm::vec3 angleToRotate;
	bool eulerAngleIncrement;
};

#endif
