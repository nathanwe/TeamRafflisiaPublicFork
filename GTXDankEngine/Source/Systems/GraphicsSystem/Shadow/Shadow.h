#ifndef SHADOW_H
#define SHADOW_H

#include "pch.h"
class Shader;
class Light;

class Shadow
{
public:
	Shadow() = default;

	// delete copy and assignment constructor
	Shadow(const Shadow&) = delete;
	Shadow& operator= (const Shadow&) = delete;

	bool Init(unsigned int shadowMapWidth, unsigned int shadowMapHeight);
	void Destroy();

	// LightDir: directional light direction
	void Update();
	inline unsigned int GetShadowMap() const { return ColorBuffer; }
	inline unsigned int GetDepthBuffer() const {return DepthBuffer;}
	inline glm::mat4 GetLightSpaceMatrix() const { return LightSpaceMatrix; }

private:
	unsigned int DepthBuffer, ColorBuffer, FBO;
	float NearPlane = 1.0f;
	float FarPlane = 50.0f;
	float Range = 15;

	// projection * view
	glm::mat4 LightSpaceMatrix;

	Shader* DefaultShader;


	void Render(Shader* shader) const;
};

#endif