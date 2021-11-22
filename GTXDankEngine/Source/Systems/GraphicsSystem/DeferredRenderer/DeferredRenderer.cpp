#include "pch.h"
#include "DeferredRenderer.h"
#include "../utils/Log.h"
#include "../Core/Shader.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"

#include "../Quad.h"


bool DeferredRenderer::Init(unsigned int gBufferWidth, unsigned int gBufferHeight)
{
	DeferredLightingShader = new Shader("Source/Shaders/DeferredRenderer/DeferredLighting.shader");
	Fill_G_BufferShader = new Shader("Source/Shaders/DeferredRenderer/Fill_G_Buffer.shader");
	CelShader = new Shader("Source/Shaders/DeferredRenderer/Cel.shader");

	// Create G buffer
	// G buffer has one FBO and 3 color attachments
	// G buffer has one depth attachment

	CreateFBO();
	CreateAlbedoMetallicBuffer(GL_COLOR_ATTACHMENT0, gBufferWidth, gBufferHeight);
	CreatePositionBuffer(GL_COLOR_ATTACHMENT1, gBufferWidth, gBufferHeight);
	CreateNormalRoughnessBuffer(GL_COLOR_ATTACHMENT2, gBufferWidth, gBufferHeight);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	// multiple render targets
	unsigned int G_Buffer[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, G_Buffer);

	CreateDepthBuffer(gBufferWidth, gBufferHeight);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_ERROR("G buffer failed to create.");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}


void DeferredRenderer::Destroy()
{
	delete DeferredLightingShader;
	delete Fill_G_BufferShader;
	delete CelShader;
}



void DeferredRenderer::CreateFBO(void)
{
	glGenFramebuffers(1, &G_Buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, G_Buffer);
}


void DeferredRenderer::CreateAlbedoMetallicBuffer(GLenum attachment, unsigned int gBufferWidth, unsigned int gBufferHeight)
{
	// Albedo + Metallic
	glGenTextures(1, &G_AlbedoMetallic);
	glBindTexture(GL_TEXTURE_2D, G_AlbedoMetallic);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gBufferWidth, gBufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, G_AlbedoMetallic, 0);
}


void DeferredRenderer::CreatePositionBuffer(GLenum attachment, unsigned int gBufferWidth, unsigned int gBufferHeight)
{
	// position
	glGenTextures(1, &G_Position);
	glBindTexture(GL_TEXTURE_2D, G_Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gBufferWidth, gBufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, G_Position, 0);
}


void DeferredRenderer::CreateNormalRoughnessBuffer(GLenum attachment, unsigned int gBufferWidth, unsigned int gBufferHeight)
{
	// Normal + Roughness
	glGenTextures(1, &G_NormalRoughness);
	glBindTexture(GL_TEXTURE_2D, G_NormalRoughness);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gBufferWidth, gBufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, G_NormalRoughness, 0);
}


void DeferredRenderer::CreateDepthBuffer(unsigned int gBufferWidth, unsigned int gBufferHeight)
{
	glGenRenderbuffers(1, &RboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gBufferWidth, gBufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RboDepth);
}


void DeferredRenderer::Bind_G_Buffer(Shader* shader)
{
	shader->setTexture("gPosition", G_Position);
	shader->setTexture("gAlbedoMetallic", G_AlbedoMetallic);
	shader->setTexture("gNormalRoughness", G_NormalRoughness);
}


void DeferredRenderer::Fill_G_Buffer(glm::mat4 view, glm::mat4 projection) 
{
	glBindFramebuffer(GL_FRAMEBUFFER, G_Buffer);     // Bind to G-Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Fill_G_BufferShader->setMat4("projection", projection);
	Fill_G_BufferShader->setMat4("view", view);


	std::set<Entity> Entitys = MaterialComponentPool.Get3SharedEntitys(TransformComponentPool.componentList, ModelComponentPool.componentList);
	for (auto e : Entitys)
	{
		auto matComponent = MaterialComponentPool.GetComponentByEntity(e);
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
		auto modelComponent = ModelComponentPool.GetComponentByEntity(e);

		// PBR and opaque
		if (matComponent->material.wireMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Fill_G_BufferRender(&matComponent->material, &transformComponent->transform, modelComponent->model->GetPointer());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (matComponent->material.IsPBR && matComponent->material.Alpha == 1.0f)
			Fill_G_BufferRender(&matComponent->material, &transformComponent->transform, modelComponent->model->GetPointer());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DeferredRenderer::Render(glm::vec3 camPos, Shadow& shadow, Shader* shader)
{
	Bind_G_Buffer(shader);

	shader->setVec3("camPos", camPos);
	shader->setTexture("ShadowMap", shadow.GetDepthBuffer());
	shader->setMat4("LightSpaceMatrix", shadow.GetLightSpaceMatrix());
	shader->setInt("hasShadow", 1);
	shader->setInt("EnablePCF", EnablePCF);

	Quad().Draw(*shader);
}

void DeferredRenderer::Render(glm::vec3 camPos, Shadow& shadow, GLuint fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (EnableCelShading)
	{
		CelShader->setFloat("fraction", CelFraction);
		Render(camPos, shadow, CelShader);
	}
	else
		Render(camPos, shadow, DeferredLightingShader);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void DeferredRenderer::Fill_G_BufferRender(Material* mat, VQS* transform, Model* model)
{
	Fill_G_BufferShader->setTexture("material.texture_albedo", mat->Albedo->GetPointer()->GetID());
	Fill_G_BufferShader->setTexture("material.texture_metallic", mat->Metallic->GetPointer()->GetID());
	Fill_G_BufferShader->setTexture("material.texture_normal", mat->Normal->GetPointer()->GetID());
	Fill_G_BufferShader->setTexture("material.texture_roughness", mat->Roughness->GetPointer()->GetID());

	Fill_G_BufferShader->setMat4("model", transform->Matrix());
	model->Draw(*Fill_G_BufferShader);
}


void DeferredRenderer::CopyDepthBufferToTarget(GLuint fbo, unsigned int gBufferWidth, unsigned int gBufferHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, G_Buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); // write to default framebuffer
	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	glBlitFramebuffer(0, 0, gBufferWidth, gBufferHeight, 0, 0, gBufferWidth, gBufferHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}




Shader* DeferredRenderer::GetLightShader()
{ 
	return EnableCelShading ? CelShader : DeferredLightingShader; 
}