#include "pch.h"

#include "GraphicsSystem.h"
#include "../../utils/Log.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../UISystem/UISystem.h"

#include "../ProfileSystem/ProfileSystem.h"
#include "Quad.h"

extern UISystem UISys;
extern Engine engine;

void GraphicsSystem::InitGLFW()

{
	if (!glfwInit()) LOG_ERROR("Failed to init the GLFW");

	LOG_INFO("GLFW init successfully");
}



void GraphicsSystem::InitWindow()
{
	// 400 is for the UI
	// will get removed when we have the UI system
	//pWindow = glfwCreateWindow(WIDTH + 400, HEIGHT, "GTX Dank AF Engine", NULL, NULL);
	pWindow = glfwCreateWindow(gsWidth, gsHeight, "GTX Dank AF Engine", NULL, NULL);
	engine.window = pWindow;
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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

	glfwSwapInterval(0);

	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// back face culling
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, gsWidth, gsHeight);

	camera.Init();

	Shadow.Init(gsWidth, gsHeight);

	DeferredRender.Init(camera.width, camera.height);

	HdrFBO.Init(camera.width, camera.height);

	//TransparentRenderer.Init(camera.width, camera.height);

	skybox.Init();

	Sky.initialize();

	PS.Init(100000);

	PostProcesser.Init(camera.width, camera.height);
	postProcessType = PostProcessType::STANDARD;
	//SetPostProcessType(PostProcessType::NEON);

	DebugRenderer.Init(&camera);

	engine.CommandSys.GetCommand("GraphicsDebugMode").SetActionToExecute([&]()
		{
			RenderingDebugMode = !RenderingDebugMode;
		});

	MenuSystem.Init();

	return true;
}




void GraphicsSystem::Update(float timeStamp)
{
	/// check if window has been resized
	int nWidth, nHeight;
	glfwGetWindowSize(engine.window, &nWidth, &nHeight);
	if (gsWidth != nWidth || gsHeight != nHeight)
	{
		gsWidth = nWidth;
		gsHeight = nHeight;
		AdjustForWindowSize();
	}
	

	PROFILE_THIS("Graphics Update");

	// clear default framebuffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// update viewport with proper width and height from camera
	glViewport(0, 0, camera.width, camera.height);

	// Render
	if (!RenderingDebugMode) Render(timeStamp);
	else DebugDraw();
	
	MenuSystem.Draw();
}



void GraphicsSystem::Render(float timeStamp)
{
	glm::mat4 view = camera.GetViewMat();
	glm::mat4 proj = camera.GetProjMat(45.0f, 0.1f, 300.0f);

	// Render shadow map
	Shadow.Update();

	// Geometry pass for G-buffer
	DeferredRender.Fill_G_Buffer(view, proj, camera.width, camera.height);

	BindLightSource(DeferredRender.GetLightShader());

	// clear hdr FBO
	// resize HDR size it may resize
	HdrFBO.Init(camera.width, camera.height);
	glBindFramebuffer(GL_FRAMEBUFFER, HdrFBO.GetFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// PBR rendering, all local pass
	DeferredRender.Render(camera.GetPosition(), Shadow, HdrFBO.GetFBO());

	DeferredRender.CopyDepthBufferToTarget(HdrFBO.GetFBO(), camera.width, camera.height);

	glDisable(GL_CULL_FACE);
	PS.Draw(timeStamp, view, proj, HdrFBO.GetFBO());
	glEnable(GL_CULL_FACE);


	Sky.Render(view, proj, HdrFBO.GetFBO());

	// post processing
	PostProcesser.Render(HdrFBO, postProcessType, camera.width, camera.height);
}




bool GraphicsSystem::Destroy()
{
	DebugRenderer.Destroy();

	PostProcesser.Destroy();

	DeferredRender.Destroy();

	//TransparentRenderer.Destroy();

	Sky.Destroy();
	skybox.Destroy();
	Shadow.Destroy();

	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return true;
}

void GraphicsSystem::BindLightSource(Shader* shader)
{
	shader->setInt("hasDirectionalLight", 0);
	unsigned int lightIndex = 0;
	std::set<Entity> LTEntitys = LightComponentPool.Get2SharedEntitys(TransformComponentPool.componentList);
	for (auto e : LTEntitys)
	{
		auto lightComponent = LightComponentPool.GetComponentByEntity(e);
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
	
		if (lightComponent->LightSource.Type == LightType::Directional)
			BindDirectionalLight(shader, &lightComponent->LightSource, &transformComponent->transform);
		else
			BindPointLight(shader, &lightComponent->LightSource, &transformComponent->transform, lightIndex);
		
		++lightIndex;
	}
	shader->setInt("numberOfLights", lightIndex);
}


void GraphicsSystem::SetSunDir(glm::vec3 dir)
{
	std::set<Entity> LTEntitys = LightComponentPool.Get2SharedEntitys(TransformComponentPool.componentList);
	for (auto e : LTEntitys)
	{
		auto lightComponent = LightComponentPool.GetComponentByEntity(e);
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);

		if (lightComponent->LightSource.Type == LightType::Directional)
			transformComponent->transform.position = glm::normalize(-dir) * glm::vec3(10) + lightComponent->LightSource.Target;
	}

}


void GraphicsSystem::BindPointLight
(Shader* shader, Light* light, VQS* transform, unsigned int index)
{
	shader->setVec3("lightPositions[" + std::to_string(index) + "]", transform->position);
	shader->setVec3("lightColors[" + std::to_string(index) + "]", light->Color * light->Intensity);
}


void GraphicsSystem::BindDirectionalLight(Shader* shader, Light* light, VQS* transform)
{
	shader->setInt("hasDirectionalLight", 1);

	// pointing at light source position
	glm::vec3 lightDirection = transform->position - light->Target;

	shader->setVec3("directionalLightDirection", glm::normalize(lightDirection));
	shader->setVec3("directionalLightColor", light->Color * light->Intensity);
}


void GraphicsSystem::DebugDraw()
{
	DebugRenderer.DrawWireFrameObj();
	if (DebugRenderer.EnableNormalVisual) DebugRenderer.DrawNormalVec();

	skybox.Render(camera.GetViewMat(), camera.GetProjMat(45.0f, 0.1f, 100.0f), 0);

	DebugRenderer.RenderLightSource(0);
	DebugRenderer.DrawCollider();
}


void GraphicsSystem::RenderUI(void)
{
	ImGui::Begin("Metallic");
	{
		ImGui::BeginChild("image");
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();

		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)(UIntToPtr(DeferredRender.GetAlbedoMetallic())), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();



	ImGui::Begin("Shadow Map");
	{
		ImGui::BeginChild("image");
		ImVec2 wsize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID)(UIntToPtr(Shadow.GetDepthBuffer())), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::Begin("Post Process");
	{
		ImGui::Checkbox("Enable HDR", &(PostProcesser.HasHDR));
		ImGui::SliderFloat("Exposure", &(PostProcesser.Exposure), 0.01f, 5.0f);
	}
	ImGui::End();

	ImGui::Begin("Render Configuration");
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0 / double(ImGui::GetIO().Framerate), double(ImGui::GetIO().Framerate));

		ImGui::Checkbox("Enable Debug Mode", &RenderingDebugMode);
		ImGui::Checkbox("Visualize Normal Vec", &DebugRenderer.EnableNormalVisual);
		ImGui::Checkbox("Enable PCF", &(DeferredRender.EnablePCF));
		ImGui::SliderFloat("Cel Fractor", &(DeferredRender.CelFraction), 0.01f, 10.0f);

		float turbidity = 0.0f;
		turbidity = Sky.turbidity();

		ImGui::SliderFloat("Turbidity", &turbidity, 2.0f, 30.0f);
		Sky.set_turbidity(turbidity);
	
		ImGui::SliderAngle("Sun Angle", &m_sun_angle, -1, -179.0f);
		glm::vec3 sunDir = glm::normalize(glm::vec3(0.0f, sin(m_sun_angle), cos(m_sun_angle)));
		SetSunDir(sunDir);
	}
	ImGui::End();

	ImGui::Begin("Camera Configs");
	{
		ImGui::Checkbox("Third Person", &camera.thirdPerson);
		ImGui::SliderFloat("Offset", &camera.thirdPersonOffset, 0.f, 20.f);
		ImGui::Checkbox("Follow Pokeball", &camera.objectTrack);
	}
	ImGui::End();
}


void GraphicsSystem::AdjustForWindowSize()
{
	camera.width = gsWidth;
	camera.height = gsHeight;

	MenuSystem.AdjustForWindowSize();
}



