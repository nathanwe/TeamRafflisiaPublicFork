#include "pch.h"
#include "PostProcess.h"
#include "../Quad.h"
#include "../FBO.h"
#include "../Core/ResourceManager.h"

#include "../Core/Texture.h"
#include "../Core/Engine.h"

bool PostProcess::Init(unsigned int weight, unsigned int height)
{
	Standard = new Shader("Source/Shaders/PostProcess/PostProcess.shader");
	Neon = new Shader("Source/Shaders/PostProcess/Neon.shader");

	PostProcessFBO.Init(weight, height);

	return true;
}




void PostProcess::Destroy()
{
	delete Neon;
	delete Standard;
}



void PostProcess::Render(const FBO& fbo, PostProcessType type, unsigned int width, unsigned int height)
{
	// since window may resize
	// change fbo
	PostProcessFBO.Init(width, height);


	if (type == PostProcessType::STANDARD)
	{
		StandardPostProcessing(fbo);
		return;
	}

	// find correct shader base on post processing type
	Shader* postProcessShader = ChooseShader(type);

	// render post processing base on input image
	RenderPostProcess(postProcessShader, fbo);

	// add cross hair
	// gamma correction
	// tone mapping
	StandardPostProcessing(PostProcessFBO);
}



void PostProcess::RenderPostProcess(Shader* PostProcessShader, const FBO& inputFBO)
{
	// bind post process frame buffer, clean buffer
	glBindFramebuffer(GL_FRAMEBUFFER, PostProcessFBO.GetFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind input texture
	PostProcessShader->setTexture("Scene", inputFBO.GetColorAttachment());

	Quad().Draw(*PostProcessShader);

	// no need to unbind fbo
	// StandardPostProcessing() will bind to default FBO at the very beginning
}



void PostProcess::StandardPostProcessing(const FBO& fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Standard->setInt("HasHDR", HasHDR);
	Standard->setFloat("exposure", Exposure);

	Standard->setTexture("Scene", fbo.GetColorAttachment());

	// get cross hair from reource manager
	ResourceHandle<Texture>* crossHair = TextureResourceManger.GetResourceHandle("Assets/Textures/CrossHair.png");

	// bind cross hair texture
	Standard->setTexture("CrossHair", crossHair->GetPointer()->GetID());

	Quad().Draw(*Standard);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// TODO:
Shader* PostProcess::ChooseShader(PostProcessType type)
{
	if (type == PostProcessType::NEON) return Neon;
	
	// this should never be returned
	return Standard;
}