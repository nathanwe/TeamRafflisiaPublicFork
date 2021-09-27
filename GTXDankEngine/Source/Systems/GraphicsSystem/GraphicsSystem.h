#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "pch.h"

#include "../../Core/Camera.h"
#include "../../Core/Model.h"
#include "../../Core/System.h"
#include "Skybox/Skybox.h"
#include "../../Core/Texture.h"


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

private:
	Shader* shaderProgram;
	Shader* skyboxShader;

	Skybox skybox;

	Camera camera;
	
	Model *model;
	Texture *texture;

	glm::vec3 lightPos;	
};
#endif // !GRAPHICSSYSTEM_H
