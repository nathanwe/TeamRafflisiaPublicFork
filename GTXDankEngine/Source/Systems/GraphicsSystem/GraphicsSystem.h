#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "pch.h"

#include "../../Core/Camera.h"
#include "../../Core/Model.h"
#include "../../Core/System.h"
#include "Skybox/Skybox.h"
#include "../../Core/Texture.h"
#include "../Components/MaterialComponent/Material.h"
#include "../Core/Model.h"
#include "../utils/VQS.h"


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

	void PbrRender(Material* mat, VQS* transform, Model* model);
	void RenderLightSource();

	void BindLightSource(Shader* shader);

private:
	Shader* ForwardPbrShader;
	Shader* skyboxShader;
	Shader* LightSourceShader;

	Skybox skybox;

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 10.0f) };

	glm::vec3 lightPos;	
};
#endif // !GRAPHICSSYSTEM_H
