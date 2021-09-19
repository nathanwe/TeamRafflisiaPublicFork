#include "pch.h"

#include "GraphicsSystem.h"
#include "../../utils/Log.h"





bool GraphicsSystem::Init()
{
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
	pWindow = glfwCreateWindow(WIDTH, HEIGHT, "GTX Dank AF Engine", NULL, NULL);
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

	model = new Model("Assets/models/scroll/scene.gltf" );
	texture = new Texture("Assets/models/scroll/textures/lambert4SG_baseColor.png" );
	shaderProgram = new Shader("Source/Shaders/basic.vert", "Source/Shaders/basic.frag" );



	// tell the viewport
	glViewport(0, 0, WIDTH, HEIGHT);


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram->Activate();
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);

	texture->texUnit(*shaderProgram, "diffuse0", 1);
	texture->Bind(1);

	return true;
}




void GraphicsSystem::Update(float timeStamp)
{
	// draw a triangle
	// pick a pretty color
	glClearColor(0.106f, 0.204f, 0.002f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	// use the shader program
	shaderProgram->Activate();

	camera.Inputs(pWindow);
	camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

	model->Draw(*shaderProgram, camera);

	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	

	glfwSwapBuffers(pWindow);

	// do GLFW stuff
	glfwPollEvents();
}



bool GraphicsSystem::Destroy()
{
	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return true;
}

