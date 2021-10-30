#ifndef OIT_H
#define OIT_H

#include "pch.h"

class Shader;

class OIT
{
public:
	OIT() = default;
	// delete copy and assignment constructor
	OIT(const OIT&) = delete;
	OIT& operator= (const OIT&) = delete;

	bool Init(unsigned int weight, unsigned int height);
	void Destroy();
	void Render(GLenum opaqueFBO, glm::mat4 view, glm::mat4 projection, unsigned int DepthBuffer);

private:
	// Create transparentFBO
	// FBO has 2 color attachments: AccumTex and RevealTex
	// FBO has 1 depth attachment: using G-buffer depth buffer
	// Multiple Render Targets
	void CreateFBO(unsigned int weight, unsigned int height);

	// Texture
	// 4 channels, channel size: 16F
	void CreateAccumTex(unsigned int weight, unsigned int height);

	// Texture
	// 1 channel, channel size: 8
	void CreateRevealTex(unsigned int weight, unsigned int height);


	void TransparentPass(glm::mat4 view, glm::mat4 projection);
	void CompositionPass(GLenum opaqueFBO);

	void RenderTransparentObjects(glm::mat4 view, glm::mat4 projection);
	void Composite(void);

	void InitQuad(void);

private:
	unsigned int transparentFBO;
	unsigned int accumTex, revealTex;
	unsigned int quadVAO, quadVBO;

	// transparent pass
	Shader* TransparentShader;

	// composite pass
	Shader* CompositeShader;

};



#endif