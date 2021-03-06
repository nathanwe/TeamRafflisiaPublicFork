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

#if _DEBUG
	pWindow = glfwCreateWindow(camera.width, camera.height, "Ball 2 The Wall", NULL, NULL);
#else
	GLFWmonitor* mMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mMonitor);
	camera.width = mode->width;
	camera.height = mode->height;
	pWindow = glfwCreateWindow(camera.width, camera.height, "B2TW", mMonitor, NULL);
#endif // _DEBUG

	// 400 is for the UI
	// will get removed when we have the UI system
	//pWindow = glfwCreateWindow(WIDTH + 400, HEIGHT, "GTX Dank AF Engine", NULL, NULL);
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

	camera.Init();

	glViewport(0, 0, camera.width, camera.height);

	Shadow.Init(camera.width, camera.height);

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

#ifdef _DEBUG
	engine.CommandSys.GetCommand("GraphicsDebugMode").SetActionToExecute([&]()
		{
			RenderingDebugMode = !RenderingDebugMode;
		});
#endif // DEBUG

	MenuSystem.Init();

	TextRenderer.Init("Assets/Fonts/HeyYou.otf");
//	TextRenderer.Init("fonts/arial.ttf");
	textShader = new Shader("Source/Shaders/GeneralUIRenderer/TextRenderer.shader");
	textShader->setMat4("projection", glm::ortho(0.0f,float(camera.width),0.0f,float(camera.height)));


	/// Initialize logo
	logoShader = new Shader("Source/Shaders/MenuRenderer/SliderPointerShader.shader");
    // VAO
    glGenVertexArrays(1, &logoVAO);
    glBindVertexArray(logoVAO);

    unsigned int logoVBO;
    // VBO
    //VertexBuffer vbo(&quadVerticesColorTexture[0], quadVerticesColorTexture.size() * sizeof(Vertex));
    glGenBuffers(1, &logoVBO);
    glBindBuffer(GL_ARRAY_BUFFER, logoVBO);
    glBufferData(GL_ARRAY_BUFFER, logoVerts.size() * sizeof(float), logoVerts.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // vertex texture coordinates (-1,-1) to signify grey color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    auto ptrTxtr = TextureResourceManger.GetResourceHandle("Assets/Credits/DigiPen_WHITE.png");
//    logoShader->setTexture("txtr", ptrTxtr->GetPointer()->GetID());
//	ptrTxtr = TextureResourceManger.GetResourceHandle("Assets/Credits/DigiPen_WHITE_1024px.png");
//	ptrTxtr = TextureResourceManger.GetResourceHandle("Assets/Credits/DigiPen_WHITE.jpg");
	ptrTxtr = TextureResourceManger.GetResourceHandle("Assets/Credits/DigiPenLogoCORRECT.png");

	return true;
}




void GraphicsSystem::Update(float timeStamp)
{
	/// check if window has been resized
	int nWidth, nHeight;
	glfwGetWindowSize(engine.window, &nWidth, &nHeight);
	if (camera.width != nWidth || camera.height != nHeight)
	{
		camera.width = nWidth;
		camera.height = nHeight;

		RendererFboResize(camera.width, camera.height);
		MenuSystem.AdjustForWindowSize();
		textShader->setMat4("projection", glm::ortho(0.0f, float(camera.width), 0.0f, float(camera.height)));
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

//	TextRenderer.RenderText(*textShader, "Hello, World!", WIDTH / 2 - 100, HEIGHT / 2 - 50, 1.3, glm::vec3(1.0,0,0));
	if (drawLogo)
	{
		this->DrawLogo();
	}

	if (drawCredits)
	{
		this->DrawCredits();
	}
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
		ImGui::Checkbox("Follow Player", &camera.objectTrack);
	}
	ImGui::End();
}




void GraphicsSystem::RendererFboResize(unsigned int width, unsigned int height)
{
	DeferredRender.Recreate_G_Buffer(width, height);

	HdrFBO.Init(width, height);
	// since window may resize
	// change fbo
	PostProcesser.Resize(width, height);
}


void GraphicsSystem::DrawCustomText(std::string text, float scale, glm::vec2 pos, glm::vec3 color)
{
	TextRenderer.RenderText(*textShader, text, pos.x, camera.height - pos.y, scale, color);
}



void GraphicsSystem::DrawLogo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	logoShader->setMat4("projection", glm::ortho(0.0f,float(camera.width),0.0f,float(camera.height)));
    logoShader->setVec2("centerLocation", glm::vec2(camera.width/2,camera.height/2));
//	auto ptrTxtr = TextureResourceManger.GetResourceHandle("Assets/Credits/DigiPen_WHITE_1024px.png");
	logoShader->setTexture("txtr", ptrTxtr->GetPointer()->GetID());

	logoShader->Bind();
    glBindVertexArray(logoVAO);
    glDisable(GL_DEPTH_TEST);
//    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    logoShader->unBind();
}


void GraphicsSystem::DrawCredits()
{
	std::stringstream ss(credit);
	std::string name, role;
	float y = 320;
	float underScale = 1.2f;
	int nextLine = 180;
	int shift = 875;
	
	if (credit.find('-') != std::string::npos)
	{
		std::getline(ss, name, '-');
		if (name.compare("Xingyu Brandon Wang") == 0 || name.compare("Kishore Kandasamy Balakrishnan") == 0
			|| name.compare("Theodore Sherman Sharygin") == 0)
		{
			this->DrawCustomText(name, 1.0f, glm::vec2(camera.width/2-500, 200), glm::vec3(1,0,0));
		}
		else if (name.compare("TEAM RAFFLESIA PRODUCTION") == 0)
		{
			this->DrawCustomText(name, 1.5f, glm::vec2(camera.width/2-700, camera.height/2), glm::vec3(1,0,0));
		}
		else
		{
			this->DrawCustomText(name, 1.4f, glm::vec2(camera.width/2-500, 200), glm::vec3(1,0,0));
		}
		y = 440;
		underScale = 0.6f;
		nextLine = 90;
		shift = 300;
	}

    while(std::getline(ss, role, ','))
	{
	    this->DrawCustomText(role, underScale, glm::vec2(camera.width/2-shift, y), glm::vec3(1,0,0));
		if (role.compare("Hey You font found on") == 0 || role.compare("Thank you for playing our game") == 0)
			y += 100;
		else
			y += nextLine;
	}
}


