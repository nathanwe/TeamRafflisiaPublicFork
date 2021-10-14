#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include "pch.h"
#include "../Core/Shader.h"

class FBO;

class PostProcess
{
public:
	PostProcess() = default;
	bool Init();
	void Destroy();

	void Render(const FBO& fbo);

	bool HasHDR = true;
	float Exposure = 1.0;

private:

	//void ToneMapping();
	Shader* shader;
	//bool HasHDR = true;


};



#endif