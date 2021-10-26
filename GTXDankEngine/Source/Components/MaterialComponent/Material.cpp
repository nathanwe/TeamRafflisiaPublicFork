#include "pch.h"
#include "Material.h"
#include "../utils/Log.h"

Material::Material(ResourceHandle<Texture>* albedo)
	: Albedo(albedo), Metallic(nullptr), Normal(nullptr), Roughness(nullptr), IsPBR(false), Alpha(1.0)
{
	if (albedo == nullptr)
		LOG_ERROR("Create Material Error: albedo texture is nullptr");
}



Material::Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, ResourceHandle<Texture>* normal, ResourceHandle<Texture>* roughness, float a)
	:Albedo(albedo), Metallic(metallic), Normal(normal), Roughness(roughness), IsPBR(false), Alpha(a)
{
	if (albedo == nullptr)
		LOG_ERROR("Create Material Error: albedo texture is nullptr");

	if (Albedo != nullptr &&
		Metallic != nullptr &&
		Roughness != nullptr &&
		Normal != nullptr)
	{
		IsPBR = true;
	}
}

/*
Material::Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, ResourceHandle<Texture>* normal, ResourceHandle<Texture>* roughness, float a)
	:Albedo(albedo), Metallic(metallic), Normal(normal), Roughness(roughness), IsPBR(false), Alpha(a)
{
	if (albedo == nullptr)
		LOG_ERROR("Create Material Error: albedo texture is nullptr");

	if (Albedo != nullptr &&
		Metallic != nullptr &&
		Roughness != nullptr &&
		Normal != nullptr)
	{
		IsPBR = true;
	}
}
*/