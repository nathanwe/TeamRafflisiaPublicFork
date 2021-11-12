#include "pch.h"
#include "UISystem.h"
#include "../ProfileSystem/ProfileSystem.h"
#include "../Core/Engine.h"

UISystem UISys;
extern ProfileSystem ProfileSys;
extern Engine engine;

bool UISystem::Init()
{
	//Initializes Imgui for OpenGL
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(openGLwindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	return true;
}


void UISystem::Update(float timeStamp)
{
	Timer timer("UI Update");

	if (engine.getMenuMode() || engine.getDebugMode())
	{
		if (engine.getDebugMode())
		{
			// Renders UI for each system
			ProfileSys.Update(0);		
			engine.GraphicsSys.RenderUI();
		}

		if (engine.getMenuMode())
		{
			engine.MenuSys.Update(0);
		}
	}
	
}

bool UISystem::Destroy()
{
	//Destroys Imgui Stuff
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void UISystem::GrabWindow(GLFWwindow* window)
{
	//This function grabs the active window from openGL
	openGLwindow = window;
}

