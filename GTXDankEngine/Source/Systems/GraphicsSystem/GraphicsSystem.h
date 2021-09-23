#pragma once


#include "pch.h"
#include "../../Core/Shader.h"
#include "../../Core/Camera.h"
#include "../../Components/ModelComponent/Model.h"
#include "../../Core/Texture.h"
#include "../../Core/System.h"
#include "Skybox/Skybox.h"


const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 1200;



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

	unsigned int LoadCubemap(std::vector<std::string> faces);
	void RenderSkybox(Shader *shader, glm::mat4 view, glm::mat4 projection);


private:
	Shader* shaderProgram;
	Shader* skyboxShader;

	Skybox skybox;

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f) };
	
	Model *model;
	Texture *texture;

	glm::vec3 lightPos;	
};