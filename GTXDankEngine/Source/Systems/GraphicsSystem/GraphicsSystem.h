#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "pch.h"

#include "../../Core/Camera.h"
#include "../../Core/System.h"
#include "Skybox/Skybox.h"
#include "Shadow/Shadow.h"
#include "DeferredRenderer/DeferredRenderer.h"
#include "FBO.h"
#include "PostProcess/PostProcess.h"
#include "DebugRender/DebugRender.h"
#include "OIT/OIT.h"


class Light;
class VQS;


const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 860;



class GraphicsSystem : public System
{
	
public:
	// default constructor
	GraphicsSystem() = default;
	
	// delete copy and assignment constructor
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator= (const GraphicsSystem&) = delete;


public:
	bool Init();

	void Update(float timeStamp);

	bool Destroy();

	GLFWwindow* pWindow;

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 10.0f) };
private:
	void InitGLFW();
	void InitWindow();
		
	void Render();

	void BindLightSource(Shader* shader);
	void BindPointLight(Shader* shader, Light* light, VQS *transform, unsigned int index);
	void BindDirectionalLight(Shader* shader, Light* light, VQS* transform);


	void DebugDraw();

	void RenderGraphicsUI(void);


private:

	Skybox skybox;

	Shadow Shadow;

	

	DeferredRenderer DeferredRender;

	// post process and forward rendering FBO
	FBO HdrFBO;

	OIT TransparentRenderer;

	PostProcess PostProcesser;

	DebugRender DebugRenderer;

	bool DebugMode = false;
};
#endif // !GRAPHICSSYSTEM_H
