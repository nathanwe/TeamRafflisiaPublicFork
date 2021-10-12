#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

#include "pch.h"

#include "../Components/MaterialComponent/Material.h"
#include "../Core/Model.h"
#include "../utils/VQS.h"

// forward declearation of shader class
class Shader;


class DeferredRenderer
{
public:
	DeferredRenderer() = default;

	// delete copy and assignment constructor
	DeferredRenderer(const DeferredRenderer&) = delete;
	DeferredRenderer& operator= (const DeferredRenderer&) = delete;

	bool Init(unsigned int gBufferWidth, unsigned int gBufferHeight);
	void Destroy();

	void Fill_G_Buffer(glm::mat4 view, glm::mat4 projection);
	void Render(glm::vec3 camPos);

	inline Shader* GetLightShader() { return DeferredLightingShader; }

	void CopyDepthBufferToTarget(GLuint fbo, unsigned int gBufferWidth, unsigned int gBufferHeight);
	inline unsigned int GetNormalRoughness() { return G_NormalRoughness; }
	inline unsigned int GetAlbedoMetallic() { return G_AlbedoMetallic; }
	inline unsigned int GetDepth() {return RboDepth; }
	 
private:
	Shader* DeferredLightingShader;
	Shader* Fill_G_BufferShader;

	unsigned int G_Buffer, G_Position, G_NormalRoughness, G_AlbedoMetallic;
	unsigned int RboDepth;	// depth buffer

	void CreateFBO(void);

	// using Texture as render target / colorbuffer 
	void CreateAlbedoMetallicBuffer(GLenum attachment, unsigned int gBufferWidth, unsigned int gBufferHeight);

	void CreatePositionBuffer(GLenum attachment, unsigned int gBufferWidth, unsigned int gBufferHeight);

	void CreateNormalRoughnessBuffer(GLenum attachment, unsigned int gBufferWidth, unsigned int gBufferHeight);

	void CreateDepthBuffer(unsigned int gBufferWidth, unsigned int gBufferHeight);

	void Bind_G_Buffer(Shader* shader);

	void Fill_G_BufferRender(Material* mat, VQS* transform, Model* model);
};



#endif