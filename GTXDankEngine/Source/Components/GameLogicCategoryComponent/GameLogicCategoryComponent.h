#ifndef GAMELOCGICCATEGORYCOMPONENT_H
#define GAMELOCGICCATEGORYCOMPONENT_H

#include "pch.h"
#include "../utils/VQS.h"
#include "../Core/ComponentPool.h"
#include <set>
#include <vector>
#include "../../Core/Enums.h"
#include "../../Core/Engine.h"
#include "../Core/ComponentPool.h"


class GameLogicCategoryComponent : public AbstractComponent
{
public:
	GameLogicCategoryComponent(Entity e, std::vector<GameLogicCategories> startCategories);

	std::set<GameLogicCategories> categories;
};

extern AbstractComponentPool<GameLogicCategoryComponent> GameLogicCategoryComponentPool;


#endif 



