#pragma once

#include "pch.h"
#include "../utils/common.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class GameObjectFactory
{
public:
	bool Init();
	Entity CreateObject(GameLogicCategories objectType);
	void SaveObject(GameLogicCategories objectType, Entity entity);
	
	//todo Load Scene and save scene in sceneManager
	//Entity LoadScene(std::string name);
	//Entity SaveScene(std::string name);

private:
	std::unordered_map<std::string, ordered_json> archetypes;
	void (*DeserializeFunctions[ComponentType::TOTAL])(ordered_json j, Entity e);
};