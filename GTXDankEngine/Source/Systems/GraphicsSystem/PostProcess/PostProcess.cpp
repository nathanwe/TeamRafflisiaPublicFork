#include "pch.h"
#include "PostProcess.h"
#include "../Quad.h"
#include "../FBO.h"


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
	Quad().Draw(*shader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}