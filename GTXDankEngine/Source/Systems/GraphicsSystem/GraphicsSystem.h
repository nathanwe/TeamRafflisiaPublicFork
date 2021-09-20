#pragma once


#include "../../Core/Shader.h"
#include "../../Core/Camera.h"
#include "../../Components/ModelComponent/Model.h"
#include "../../Core/Texture.h"
#include "../../Core/System.h"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;



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
		


private:
	Shader* shaderProgram;

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f) };
	
	Model *model;
	Texture *texture;

	glm::vec3 lightPos;
	
};