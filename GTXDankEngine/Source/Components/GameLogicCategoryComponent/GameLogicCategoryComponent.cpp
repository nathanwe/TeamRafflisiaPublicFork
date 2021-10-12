#include "pch.h"

#include "../utils/Log.h"
#include "GameLogicCategoryComponent.h"

AbstractComponentPool<GameLogicCategoryComponent> GameLogicCategoryComponentPool;

GameLogicCategoryComponent::GameLogicCategoryComponent(Entity e, std::vector<GameLogicCategories> startCategories)
	:AbstractComponent{ entity }
{
	for (GameLogicCategories cat : startCategories)
	{
		categories.insert(cat);
	}
	LOG_INFO("Creating GameLogicCategoryComponent to entity: {}", e);
}


