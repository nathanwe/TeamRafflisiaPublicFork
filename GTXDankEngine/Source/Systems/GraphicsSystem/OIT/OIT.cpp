#include "pch.h"
#include "OIT.h"
#include "../utils/Log.h"
#include "../Core/Shader.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"

bool OIT::Init(unsigned int weight, unsigned int height)
{
	CreateFBO(weight, height);

	InitQuad();

	TransparentShader = new Shader("Source/Shaders/OIT/TransparentPass.shader");
	CompositeShader = new Shader("Source/Shaders/OIT/CompositionPass.shader");

	return true;
}


void OIT::Destroy()
{
	delete TransparentShader;
	delete CompositeShader;

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);

	glDeleteTextures(1, &accumTex);
	glDeleteTextures(1, &revealTex);
	glDeleteFramebuffers(1, &transparentFBO);
}


void OIT::CreateFBO(unsigned int weight, unsigned int height)
{
	glGenFramebuffers(1, &transparentFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);

	CreateAccumTex(weight, height);
	CreateRevealTex(weight, height);

	// Multiple Render Targets
	const GLenum transparentDrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, transparentDrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_ERROR("Transparent buffer failed to create.");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OIT::CreateAccumTex(unsigned int weight, unsigned int height)
{
	glGenTextures(1, &accumTex);
	glBindTexture(GL_TEXTURE_2D, accumTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, weight, height, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTex, 0);
}



void OIT::CreateRevealTex(unsigned int weight, unsigned int height)
{
	glGenTextures(1, &revealTex);
	glBindTexture(GL_TEXTURE_2D, revealTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, weight, height, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTex, 0);
}


void OIT::Render(GLenum opaqueFBO, glm::mat4 view, glm::mat4 projection, unsigned int DepthBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
	// attach G-buffer depth buffer to transparentFBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);

	TransparentPass(view, projection);

	CompositionPass(opaqueFBO);
}


void OIT::TransparentPass(glm::mat4 view, glm::mat4 projection)
{
	// configure render states
	// disable depth writes so transparent objects wouldn't interfere with solid pass depth values
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	
	// Set up default alpha channel values for multiple render target
	glBlendFunci(0, GL_ONE, GL_ONE); // accumulation blend target
	glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR); // revealge blend target
	glBlendEquation(GL_FUNC_ADD);

	// clear FBO
	// use a four component float array or a glm::vec4(0.0)
	float zeroFillerVec[4] = { 0.0, 0.0, 0.0, 0.0 };
	glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);

	// use a four component float array or a glm::vec4(1.0)
	float oneFillerVec[4] = { 1.0, 1.0, 1.0, 1.0 };
	glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);

	// Render all transparent objects
	RenderTransparentObjects(view, projection);
}


void OIT::CompositionPass(GLenum opaqueFBO)
{
	// set render states
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind opaque framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);

	// composite opaque layer and transparent layer.
	// ccumulation buffer tells the color 
	// revealage buffer determines the visibility of the the underlying pixel: 
	Composite();

	// reset render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE); // enable depth writes so glClear won't ignore clearing the depth buffer
	glDisable(GL_BLEND);
}



void OIT::RenderTransparentObjects(glm::mat4 view, glm::mat4 projection)
{
	TransparentShader->setMat4("projection", projection);
	TransparentShader->setMat4("view", view);


	std::set<Entity> Entitys = MaterialComponentPool.Get3SharedEntitys(TransformComponentPool.componentList, ModelComponentPool.componentList);
	for (auto e : Entitys)
	{
		auto matComponent = MaterialComponentPool.GetComponentByEntity(e);
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
		auto modelComponent = ModelComponentPool.GetComponentByEntity(e);

		// if it's transparent object, bind diffuse texture, bind alpha, bind model matrix
		if (matComponent->material->Alpha != 1.0f)
		{
			TransparentShader->setTexture("diffuse", matComponent->material->Albedo->GetPointer()->GetID());
			TransparentShader->setFloat("alpha", matComponent->material->Alpha);
			TransparentShader->setMat4("model", transformComponent->transform->Matrix());

			modelComponent->model->GetPointer()->Draw(*TransparentShader);	// Draw call
		}
	}
}


void OIT::Composite(void)
{
	CompositeShader->setTexture("accum", accumTex);
	CompositeShader->setTexture("reveal", revealTex);

	CompositeShader->Bind();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	CompositeShader->unBind();
}



void OIT::InitQuad(void)
{
	float quadVertices[] = {
		// positions		// uv
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}