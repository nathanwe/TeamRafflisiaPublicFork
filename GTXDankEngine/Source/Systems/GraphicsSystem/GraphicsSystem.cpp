#include "pch.h"

#include "GraphicsSystem.h"
#include "../../utils/Log.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Core/Engine.h"
#include "../Core/ComponentPool.cpp"


#include "../UISystem/UISystem.h"
#include "../ProfileSystem/ProfileSystem.h"

extern UISystem UISys;
extern ProfileSystem ProfileSys;

//#include <yaml/yaml.h>


extern std::vector<Entity> EntityList;

void GraphicsSystem::InitGLFW()

{
	if (!glfwInit()) LOG_ERROR("Failed to init the GLFW");

	LOG_INFO("GLFW init successfully");
}



void GraphicsSystem::InitWindow()
{
	pWindow = glfwCreateWindow(WIDTH, HEIGHT, "GTX Dank AF Engine", NULL, NULL);

	if (pWindow == NULL)
	{
		LOG_ERROR("Failed to create the GLFW window");

		glfwTerminate();
	}

	LOG_INFO("GLFW window init successfully");

	glfwMakeContextCurrent(pWindow);
}



bool GraphicsSystem::Init()
{
	InitGLFW();

	InitWindow();

	// use glad
	gladLoadGL();

	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// tell the viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	
	std::vector<std::string> faces
	{
			"Assets/Textures/Skybox/sky/right.jpg",
			"Assets/Textures/Skybox/sky/left.jpg",
			"Assets/Textures/Skybox/sky/top.jpg",
			"Assets/Textures/Skybox/sky/bottom.jpg",
			"Assets/Textures/Skybox/sky/front.jpg",
			"Assets/Textures/Skybox/sky/back.jpg"
	};

	skybox.Init(faces);
	
	shaderProgram = new Shader("Source/Shaders/basic.shader");
	skyboxShader = new Shader("Source/Shaders/Skybox/Skybox.shader");
	
	// test if yaml lib is linked properly
	//YAML::Emitter out;
	

	//
	Model* model = new Model("Assets/models/PokemonBall/model.obj" );
	Texture* texture = new Texture("Assets/models/PokemonBall/albedo.jpg" );
	

	// Add model component
	Entity a;
	Entity entity = 1;
	EntityList.push_back(a);
	EntityList.push_back(entity);

	ModelComponentPool.Add(entity, ( model ));


	// Pass uniforms to shader
	lightPos = glm::vec3(1.0f, 1.0f, 1.0f);

	shaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	shaderProgram->setTexture("diffuse0", texture->GetID());
	
	glm::mat4 modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.01));
	shaderProgram->setMat4("model", modelMat);

	return true;
}




void GraphicsSystem::Update(float timeStamp)
{
	glClearColor(0.106f, 0.204f, 0.002f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update camera
	camera.Inputs(pWindow);
	camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

	// Render for graphics
	Render();

	// render UI
	UISys.Update(0);

	glfwSwapBuffers(pWindow);

	glfwPollEvents();
}



void GraphicsSystem::Render()
{
	shaderProgram->setVec3("lightPos", lightPos);
	shaderProgram->setVec3("camPos", camera.Position);

	shaderProgram->setMat4("view", camera.GetViewMat());
	shaderProgram->setMat4("projection", camera.GetProjMat(45.0f, 0.1f, 300.0f));
	
	for (const auto& [entity, modelComponent] : ModelComponentPool.componentList) {
		modelComponent->model->Draw(*shaderProgram);
	}
	
	skybox.Render(skyboxShader, camera.GetViewMat(), camera.GetProjMat(45.0f, 0.1f, 100.0f));
}



bool GraphicsSystem::Destroy()
{
	skybox.Destroy();
	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return true;
}


