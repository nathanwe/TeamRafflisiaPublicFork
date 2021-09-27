#include "pch.h"
#include "Material.h"
#include "../utils/Log.h"

Material::Material(Texture* albedo)
	: Albedo(albedo), Metallic(nullptr), Normal(nullptr), Roughness(nullptr), IsPBR(false)
{
	if (albedo == nullptr)
		LOG_ERROR("Create Material Error: albedo texture is nullptr");
}



Material::Material(Texture* albedo, Texture* metallic, Texture* normal, Texture* roughness)
	:Albedo(albedo), Metallic(metallic), Normal(normal), Roughness(roughness), IsPBR(false)
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
