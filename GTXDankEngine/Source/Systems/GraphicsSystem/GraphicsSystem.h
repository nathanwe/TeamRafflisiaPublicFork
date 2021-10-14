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

private:
	void InitGLFW();
	void InitWindow();
		
	void Render();

	void RenderLightSource(GLuint fbo);

	void BindLightSource(Shader* shader);
	void BindPointLight(Shader* shader, Light* light, VQS *transform, unsigned int index);
	void BindDirectionalLight(Shader* shader, Light* light, VQS* transform);


private:
	Shader* LightSourceShader;

	Skybox skybox;

	Shadow Shadow;

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 10.0f) };

	DeferredRenderer DeferredRender;

	// post process and forward rendering FBO
	FBO HdrFBO;

	PostProcess PostProcesser;
};
#endif // !GRAPHICSSYSTEM_H
