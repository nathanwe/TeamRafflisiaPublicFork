
#pragma once
#include "pch.h"
#include "../Core/ComponentPool.h"






class ModelComponent : public Component
{
public:
	ModelComponent(Entity entity, Model* model);

	Model* model;

};


extern ComponentPool<ModelComponent> ModelComponentPool;
