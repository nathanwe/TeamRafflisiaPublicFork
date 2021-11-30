#include "pch.h"
#include "PostProcess.h"
#include "../Quad.h"
#include "../FBO.h"
#include "../Core/ResourceManager.h"

#include "../Core/Texture.h"
#include "../Core/Engine.h"

bool PostProcess::Init()
{
	shader = new Shader("Source/Shaders/PostProcess/PostProcess.shader");
	return true;
}




void PostProcess::Destroy()
{
	delete shader;
}

void PostProcess::Render(const FBO& fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader->setInt("HasHDR", HasHDR);
	shader->setFloat("exposure", Exposure);

	shader->setTexture("Scene", fbo.GetColorAttachment());

	// should be replaced by using resource manager

	ResourceHandle<Texture>* crossHair = TextureResourceManger.GetResourceHandle("Assets/Textures/CrossHair.png");
	//Texture crossHair("Assets/Textures/CrossHair.png");
	//crossHair.OnLoad();
	shader->setTexture("CrossHair", crossHair->GetPointer()->GetID());

	Quad().Draw(*shader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}