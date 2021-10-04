#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "pch.h"
#include "../Core/ComponentPool.h"
#include "../Core/ResourceManager.h"


class ModelComponent : public AbstractComponent
{
public:
	ModelComponent(Entity entity, ResourceHandle<Model>* model);

	ResourceHandle<Model>* model;
};

extern AbstractComponentPool<ModelComponent> ModelComponentPool;
extern ResourceManager<Model> ModelResourceManager;

#endif // !MODELCOMPONENT_H
