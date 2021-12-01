#ifndef GAMELOCGICCATEGORYCOMPONENT_H
#define GAMELOCGICCATEGORYCOMPONENT_H

#include "pch.h"

#include "../utils/VQS.h"
#include "../Core/ComponentPool.h"
#include "../../Core/Enums.h"
#include "../../Core/Engine.h"


class GameLogicCategoryComponent : public AbstractComponent
{
public:
	GameLogicCategoryComponent(Entity e, std::vector<GameLogicCategories> startCategories);

	std::set<GameLogicCategories> categories;
};

extern AbstractComponentPool<GameLogicCategoryComponent> GameLogicCategoryComponentPool;


#endif 



