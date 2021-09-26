#ifndef TRANS_COMPONENT_H
#define TRANS_COMPONENT_H

#include "pch.h"
#include "../utils/VQS.h"
#include "../Core/ComponentPool.h"

class TransformComponent : public AbstractComponent
{
public:
	TransformComponent(Entity e, VQS* transform);

	VQS* transform;	
};

extern AbstractComponentPool<TransformComponent> TransformComponentPool;

#endif 



