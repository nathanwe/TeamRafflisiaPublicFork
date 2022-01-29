#include "pch.h"
#include "Material.h"
#include "../utils/Log.h"

Material::Material(ResourceHandle<Texture>* albedo)
	: Albedo(albedo), Metallic(nullptr), Alpha(1.0)
{
	if (albedo == nullptr)
		LOG_ERROR("Create Material Error: albedo texture is nullptr");
}



Material::Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, float a)
	:Albedo(albedo), Metallic(metallic), Alpha(a)
{
	if (albedo == nullptr)
		LOG_ERROR("Create Material Error: albedo texture is nullptr");
}

