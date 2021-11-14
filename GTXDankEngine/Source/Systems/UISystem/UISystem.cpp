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

	// Render UI
	if (engine.getMenuMode() || engine.getDebugMode() || engine.getEditMode())
	{
		//now in engine.run
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		if (engine.getDebugMode())
		{
			//UISys.Update(0);
			engine.GraphicsSys.RenderUI();
		}

		if (engine.getMenuMode())
		{
			engine.MenuSys.Update(0);
		}

		if (engine.getEditMode())
		{
			engine.LevelEditorSys.UpdateGUI();
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

