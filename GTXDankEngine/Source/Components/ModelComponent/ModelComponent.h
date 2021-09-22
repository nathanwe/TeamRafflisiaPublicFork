
#pragma once
#include "pch.h"
#include "../Core/ComponentPool.h"






class ModelComponent : public AbstractComponent
{
public:
	ModelComponent(Entity entity, Model* model);

	Model* model;

};


extern AbstractComponentPool<ModelComponent> ModelComponentPool;
