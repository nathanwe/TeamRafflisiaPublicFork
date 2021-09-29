#ifndef LIGHT_COM_H
#define LIGHT_COM_H

#include "pch.h"
#include "../Core/ComponentPool.h"
#include "Light.h"


class LightComponent : public AbstractComponent
{
public:
	LightComponent(Entity e, Light* light);

	Light* LightSource;
};

extern AbstractComponentPool<LightComponent> LightComponentPool;

#endif