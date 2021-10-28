#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H



#include "pch.h"
#include "../utils/common.h"
#include "../utils/SerializationHelper.h"
#include "../Core/Enums.h"

class GameObjectFactory
{
public:
	bool Init();
	Entity CreateObject(std::string name);
	void SaveObject(std::string name, Entity entity);

private:
	std::unordered_map<std::string, ordered_json> archetypes;
	void (*DeserializeFunctions[ComponentType::TOTAL])(ordered_json j, Entity e);
};

#endif // !GAMEOBJECTFACTORY_H
