#include "pch.h"
#include "UISystem.h"
#include "../ProfileSystem/ProfileSystem.h"

UISystem UISys;
extern ProfileSystem ProfileSys;

bool UISystem::Init()
{
	//Initializes Imgui for OpenGL
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(openGLwindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	//io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
	return true;
}
void UISystem::Update(float timeStamp)
{
	//Creates the Imgui new frame
	
	Timer timer("UI Update");

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if(ImGui::IsMouseDown(ImGuiMouseButton_Right))
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	ImGui::Begin("ImGui Window");
	//every output on imgui is contained within this scope
	{	
		ProfileSys.Update(0);
	}
	//ImGui::End();
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

