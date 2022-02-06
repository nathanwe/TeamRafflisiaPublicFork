#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include "pch.h"
#include "../Core/Shader.h"
#include "../FBO.h"
#include "../Core/Enums.h"

class FBO;

class PostProcess
{
public:
	PostProcess() = default;
	bool Init(unsigned int weight, unsigned int height);
	void Destroy();

	void Render(const FBO& fbo, PostProcessType type);

	bool HasHDR = true;
	float Exposure = 1.0;

private:
	Shader* ChooseShader(PostProcessType type);

	void RenderPostProcess(Shader* PostProcessShader, const FBO& inputFBO);


	// cross hair and gamma correction and tone mapping
	// shader Standard will get invoked
	void StandardPostProcessing(const FBO& fbo);

	// basic cel shading with gamma correction and tone mapping
	Shader* Standard;

	// "Inverted color and edge detection"
	Shader* Neon;


	FBO PostProcessFBO;

	


};



#endif