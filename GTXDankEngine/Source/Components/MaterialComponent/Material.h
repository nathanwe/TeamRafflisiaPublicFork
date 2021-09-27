#ifndef MAT_H
#define MAT_H

#include "pch.h"
#include "../Core/Texture.h"

class Material
{
public:
	Material(Texture* albedo);
	Material(Texture* albedo, Texture* metallic, Texture* normal, Texture* roughness);
	
	bool IsPBR;

	Texture* Albedo;
	Texture* Metallic;
	Texture* Normal;
	Texture* Roughness;



};


#endif