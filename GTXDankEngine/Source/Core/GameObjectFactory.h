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
	Entity CreateObjectFromJson(ordered_json entityJson);
	Entity CreateObject(std::string name);
	ordered_json SerializeObject(Entity entity);
	void SaveObject(std::string name, Entity entity);
	bool SaveObject(Entity entity);
	std::vector<std::string> GetTemplateObjectNames();

private:
	std::map<std::string, ordered_json> archetypes;
	void (*DeserializeFunctions[ComponentType::TOTAL])(ordered_json j, Entity e);
	std::vector<std::string> templateObjectNames;
};

#endif // !GAMEOBJECTFACTORY_H
