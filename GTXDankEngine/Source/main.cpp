#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"


#include "Model.h"
#include "Profiler.h"
#include "utils/Log.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;



Profiler* profiler = new Profiler();

int main() 
{
	// begin
	Log::Init();

	if (!glfwInit())
	{
		LOG_ERROR("Failed to init the GLFW");
		return false;
	}
	LOG_INFO("GLFW init successfully");


	// tell GLFW what it doesn't know, we're using 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// we're also using the CORE profile, keepin' it modern
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// make the window
	GLFWwindow* pWindow = glfwCreateWindow(WIDTH, HEIGHT, "GTX Dank AF Engine", NULL, NULL);
	// Check if the window failed
	if (pWindow == NULL) 
	{
		LOG_ERROR("Failed to create the GLFW window");

		glfwTerminate();
		return -1;
	}

	LOG_INFO("GLFW window init successfully");

	// tell GLFW that yes, we'd like to use the window we just made
	glfwMakeContextCurrent(pWindow);

	// use glad
	gladLoadGL();


	// tell the viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	


	// ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// ImGUI Variables
	bool drawObject = true;


	// Creates Shader class
	Shader shaderProgram("Source/Shaders/basic.vert", "Source/Shaders/basic.frag");

	
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	
	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);

	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model("Assets/models/scroll/scene.gltf");

	Texture texture("Assets/models/scroll/textures/lambert4SG_baseColor.png", "", 1);
	
	texture.texUnit(shaderProgram, "diffuse0", 1);
	texture.Bind();

	// big loop
	while (!glfwWindowShouldClose(pWindow)) 
	{
		// draw a triangle
		// pick a pretty color
		glClearColor(0.106f, 0.204f, 0.002f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// use the shader program
		shaderProgram.Activate();

		camera.Inputs(pWindow);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		if (drawObject)
		{
			model.Draw(shaderProgram, camera);
		}



		// ImGUI window creation
		ImGui::Begin("ImGUI Window");
		// Text that appears in the window
		ImGui::Text("Sample Text");
		// Checkbox that appears in the window
		ImGui::Checkbox("Draw Object", &drawObject);
		// Slider that appears in the window
		ImGui::SliderFloat("Light X", &lightPos.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Light Y", &lightPos.y, -1.0f, 1.0f);
		ImGui::SliderFloat("Light Z", &lightPos.z, -1.0f, 1.0f);
		// color editor
		// ImGui::ColorEdit4("Color", color);
		// Ends the window
		// Plot some values
		const float my_values[] = { 60, 20, 10, 40, 100, 40 };
		ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));
		ImGui::Text("Hooray 60 FPS?");

		ImGui::End();

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
		glfwSwapBuffers(pWindow);

		// do GLFW stuff
		glfwPollEvents();
	}
	// delete everything we don't need anymore
	shaderProgram.Delete();


	// end
	Log::ShutDown();

	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return 0;
}