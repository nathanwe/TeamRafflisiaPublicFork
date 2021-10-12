#include "pch.h"

#include "GraphicsSystem.h"

#include "../../utils/Log.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"


#include "../UISystem/UISystem.h"
#include "../ProfileSystem/ProfileSystem.h"

#include "../../Core/Texture.h"

extern UISystem UISys;
extern ProfileSystem ProfileSys;


extern std::vector<Entity> EntityList;

void GraphicsSystem::InitGLFW()

{
	if (!glfwInit()) LOG_ERROR("Failed to init the GLFW");

	LOG_INFO("GLFW init successfully");
}



void GraphicsSystem::InitWindow()
{
	// 400 is for the UI
	// will get removed when we have the UI system
	pWindow = glfwCreateWindow(WIDTH + 400, HEIGHT, "GTX Dank AF Engine", NULL, NULL);

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

	// back face culling
	glEnable(GL_CULL_FACE);

	// tell the viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	

	camera.Init();

	DeferredRender.Init(camera.width, camera.height);

	skybox.Init();
	
	LightSourceShader = new Shader("Source/Shaders/LightSource.shader");
	
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

	ImGui::Begin("Metallic");
	{
		ImGui::BeginChild("image");
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();

		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)DeferredRender.GetAlbedoMetallic(), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::Begin("Normal");
	{
		ImGui::BeginChild("image");
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();

		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)DeferredRender.GetNormalRoughness(), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();


	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(pWindow);

	glfwPollEvents();
}



void GraphicsSystem::Render()
{
	// Geometry pass for G-buffer
	DeferredRender.Fill_G_Buffer(camera.GetViewMat(), camera.GetProjMat(45.0f, 0.1f, 300.0f));

	BindLightSource(DeferredRender.GetLightShader());


	// PBR rendering, all local pass
	DeferredRender.Render(camera.Position);

	// copy depth buffer from G-buffer to default FBO
	DeferredRender.CopyDepthBufferToTarget(0, camera.width, camera.height);


	RenderLightSource();

	skybox.Render(camera.GetViewMat(), camera.GetProjMat(45.0f, 0.1f, 100.0f));
}



void GraphicsSystem::RenderLightSource()
{
	for (auto e : EntityList)
	{
		auto lightComponent = LightComponentPool.GetComponentByEntity(e);
		if (lightComponent != nullptr)
		{
			auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
			if (transformComponent != nullptr)
			{
				auto modelComponent = ModelComponentPool.GetComponentByEntity(e);
				if (modelComponent != nullptr)
				{
					LightSourceShader->setVec3("lightColor", lightComponent->LightSource->Color);
					LightSourceShader->setMat4("view", camera.GetViewMat());
					LightSourceShader->setMat4("projection", camera.GetProjMat(45.0f, 0.1f, 300.0f));
					LightSourceShader->setMat4("model", transformComponent->transform->Matrix());
					modelComponent->model->GetPointer()->Draw(*LightSourceShader);
				}
			}
		}
	}
	
}



bool GraphicsSystem::Destroy()
{
	delete LightSourceShader;

	DeferredRender.Destroy();
	skybox.Destroy();
	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return true;
}

void GraphicsSystem::BindLightSource(Shader* shader)
{
	shader->setInt("hasDirectionalLight", 0);
	unsigned int lightIndex = 0;
	for (const auto& [lightEntity, lightComponent] : LightComponentPool.componentList)
	{
		for (const auto& [transEntity, transformComponent] : TransformComponentPool.componentList)
		{
			if ( lightEntity == transEntity)
			{
				if (lightComponent->LightSource->Type == LightType::Directional)
					BindDirectionalLight(shader, lightComponent->LightSource, transformComponent->transform);
				else
					BindPointLight(shader, lightComponent->LightSource, transformComponent->transform, lightIndex);
			}

			++lightIndex;
		}
	}

	shader->setInt("numberOfLights", lightIndex);
}


void GraphicsSystem::BindPointLight
(Shader* shader, Light* light, VQS* transform, unsigned int index)
{
	glm::vec3 color = light->Color;
	glm::vec3 intensity = light->Intensity;
	glm::vec3 position = transform->position;

	shader->setVec3("lightPositions[" + std::to_string(index) + "]", position);
	shader->setVec3("lightColors[" + std::to_string(index) + "]", color * intensity);
}


void GraphicsSystem::BindDirectionalLight(Shader* shader, Light* light, VQS* transform)
{
	shader->setInt("hasDirectionalLight", 1);

	glm::vec3 color = light->Color;
	glm::vec3 intensity = light->Intensity;
	glm::vec3 position = transform->position;

	// pointing at light source position
	glm::vec3 lightDirection = position - light ->Target;

	shader->setVec3("directionalLightDirection", lightDirection);
	shader->setVec3("directionalLightColor", color * intensity);
}