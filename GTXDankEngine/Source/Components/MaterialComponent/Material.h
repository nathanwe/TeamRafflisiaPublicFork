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
	Material(ResourceHandle<Texture>* albedo, ResourceHandle<Texture>* metallic, float a = 1);

	ResourceHandle<Texture>* Albedo;
	ResourceHandle<Texture>* Metallic;

	std::string AlbedoPath;
	std::string MetallicPath;
	
	float Alpha;

	bool wireMode;
};

inline void to_json(ordered_json& j, const Material& material) {
	to_json(j["AlbedoPath"], material.AlbedoPath);
	to_json(j["MetallicPath"], material.MetallicPath);
	to_json(j["Alpha"], material.Alpha);
	to_json(j["wireMode"], material.wireMode);
}

inline void from_json(const ordered_json& j, Material& material) {
	from_json(j["AlbedoPath"], material.AlbedoPath);
	from_json(j["MetallicPath"], material.MetallicPath);
	from_json(j["Alpha"], material.Alpha);
	from_json(j["wireMode"], material.wireMode);
}


#endif