#include "pch.h"

#include "GraphicsSystem.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"

#include "../UISystem/UISystem.h"
#include "../ProfileSystem/ProfileSystem.h"

#include "../../Core/Texture.h"

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
	
	//shaderProgram = new Shader("Source/Shaders/basic.shader");
	LightSourceShader = new Shader("Source/Shaders/LightSource.shader");
	ForwardPbrShader = new Shader("Source/Shaders/basicPBR.shader");
	skyboxShader = new Shader("Source/Shaders/Skybox/Skybox.shader");
	
	// test if yaml lib is linked properly
	//YAML::Emitter out;

	camera.Init();

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

	for (const auto& [modelEntity, modelComponent] : ModelComponentPool.componentList) {

		for (const auto& [transEntity, transformComponent] : TransformComponentPool.componentList)
		{
			for (const auto& [matEntity, matComponent] : MaterialComponentPool.componentList)
			{
				if (modelEntity == transEntity && matEntity == transEntity)
				{
					if (matComponent->material->IsPBR)
						PbrRender(matComponent->material, transformComponent->transform, modelComponent->model);
				}
			}
		}
	}

	RenderLightSource();
	
	skybox.Render(skyboxShader, camera.GetViewMat(), camera.GetProjMat(45.0f, 0.1f, 100.0f));
}


void GraphicsSystem::PbrRender(Material* mat, VQS* transform, Model* model)
{
	BindLightSource(ForwardPbrShader);

	ForwardPbrShader->setVec3("camPos", camera.Position);

	ForwardPbrShader->setMat4("view", camera.GetViewMat());
	ForwardPbrShader->setMat4("projection", camera.GetProjMat(45.0f, 0.1f, 300.0f));
	
	ForwardPbrShader->setTexture("albedoTex", mat->Albedo->GetID());
	ForwardPbrShader->setTexture("metallicTex", mat->Metallic->GetID());
	ForwardPbrShader->setTexture("normalTex", mat->Normal->GetID());
	ForwardPbrShader->setTexture("roughnessTex", mat->Roughness->GetID());

	ForwardPbrShader->setMat4("model", transform->Matrix());
	model->Draw(*ForwardPbrShader);
}


void GraphicsSystem::RenderLightSource()
{
	for (const auto& [lightEntity, lightComponent] : LightComponentPool.componentList)
	{
		for (const auto& [modelEntity, modelComponent] : ModelComponentPool.componentList) {

			for (const auto& [transEntity, transformComponent] : TransformComponentPool.componentList)
			{
				if (modelEntity == transEntity && lightEntity == transEntity)
				{
					LightSourceShader->setVec3("lightColor", lightComponent->LightSource->Color);
					LightSourceShader->setMat4("view", camera.GetViewMat());
					LightSourceShader->setMat4("projection", camera.GetProjMat(45.0f, 0.1f, 300.0f));
					LightSourceShader->setMat4("model", transformComponent->transform->Matrix());
					modelComponent->model->Draw(*LightSourceShader);
				}
			}
		}
	}
}



bool GraphicsSystem::Destroy()
{
	skybox.Destroy();
	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return true;
}

void GraphicsSystem::BindLightSource(Shader* shader)
{
	unsigned int lightIndex = 0;
	for (const auto& [lightEntity, lightComponent] : LightComponentPool.componentList)
	{
		for (const auto& [transEntity, transformComponent] : TransformComponentPool.componentList)
		{
			if ( lightEntity == transEntity)
			{
				glm::vec3 color = lightComponent->LightSource->Color;
				glm::vec3 intensity = lightComponent->LightSource->Intensity;

				shader->setVec3("lightPositions[" + std::to_string(lightIndex) + "]", transformComponent->transform->position);
				shader->setVec3("lightColors[" + std::to_string(lightIndex) + "]", color * intensity);
			}

			++lightIndex;
		}
	}
}
