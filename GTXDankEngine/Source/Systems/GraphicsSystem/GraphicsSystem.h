#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H




#include "pch.h"
#include "../../Core/Shader.h"
#include "../../Core/Camera.h"
#include "../../Core/Model.h"
#include "../../Core/Texture.h"
#include "../../Core/System.h"
#include "Skybox/Skybox.h"


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

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 8.0f) };
	
	//Model *model;
	//Texture *texture;

	glm::vec3 lightPos;	
};
#endif // !GRAPHICSSYSTEM_H
