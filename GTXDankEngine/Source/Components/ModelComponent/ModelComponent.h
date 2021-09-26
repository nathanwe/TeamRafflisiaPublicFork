#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "pch.h"
#include "../Core/ComponentPool.h"


class ModelComponent : public AbstractComponent
{
public:
	ModelComponent(Entity entity, Model* model);

	Model* model;
};

extern AbstractComponentPool<ModelComponent> ModelComponentPool;

#endif // !MODELCOMPONENT_H
