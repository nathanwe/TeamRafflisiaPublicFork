
#ifndef MAT_COM_H
#define MAT_COM_H

#include "pch.h"

#include "Material.h"
#include "../Core/ComponentPool.h"

class MaterialComponent : public AbstractComponent
{
public:
	MaterialComponent(Entity e, Material* mat);

	Material* material;
};

extern AbstractComponentPool<MaterialComponent> MaterialComponentPool;



#endif
