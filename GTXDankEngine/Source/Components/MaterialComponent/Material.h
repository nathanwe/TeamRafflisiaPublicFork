#ifndef MAT_H
#define MAT_H

#include "pch.h"
#include "../Core/Texture.h"
#include "../Core/ResourceManager.h"
#include "../utils/SerializationHelper.h"

class Material
{
public:
	Material(ResourceHandle<Texture>* albedo);
	Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, ResourceHandle<Texture>* normal, ResourceHandle<Texture>* roughness, float a = 1);
	//Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, ResourceHandle<Texture>* normal, ResourceHandle<Texture>* roughness, float a);

	bool IsPBR;

	ResourceHandle<Texture>* Albedo;
	ResourceHandle<Texture>* Metallic;
	ResourceHandle<Texture>* Normal;
	ResourceHandle<Texture>* Roughness;

	std::string AlbedoPath;
	std::string MetallicPath;
	std::string NormalPath;
	std::string RoughnessPath;
	float Alpha;

	//for level editor
	bool wireMode;
};

inline void to_json(ordered_json& j, const Material& material) {
	to_json(j["AlbedoPath"], material.AlbedoPath);
	to_json(j["MetallicPath"], material.MetallicPath);
	to_json(j["NormalPath"], material.NormalPath);
	to_json(j["RoughnessPath"], material.RoughnessPath);
	to_json(j["Alpha"], material.Alpha);
}

inline void from_json(const ordered_json& j, Material& material) {
	from_json(j["AlbedoPath"], material.AlbedoPath);
	from_json(j["MetallicPath"], material.MetallicPath);
	from_json(j["NormalPath"], material.NormalPath);
	from_json(j["RoughnessPath"], material.RoughnessPath);
	from_json(j["Alpha"], material.Alpha);
}


#endif