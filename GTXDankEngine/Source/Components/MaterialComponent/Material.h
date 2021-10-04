#ifndef MAT_H
#define MAT_H

#include "pch.h"
#include "../Core/Texture.h"
#include "../Core/ResourceManager.h"

class Material
{
public:
	Material(ResourceHandle<Texture>* albedo);
	Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, ResourceHandle<Texture>* normal, ResourceHandle<Texture>* roughness);
	
	bool IsPBR;

	ResourceHandle<Texture>* Albedo;
	ResourceHandle<Texture>* Metallic;
	ResourceHandle<Texture>* Normal;
	ResourceHandle<Texture>* Roughness;



};


#endif