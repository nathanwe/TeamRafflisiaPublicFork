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
	
	
	// test if yaml lib is linked properly
	//YAML::Emitter out;
	
	// scroll
	model = new Model("Assets/models/scroll/scene.gltf" );
	texture = new Texture("Assets/models/scroll/textures/lambert4SG_baseColor.png" );
	
	// map
	//model = new Model("Assets/models/map/scene.gltf");
	//texture = new Texture("Assets/models/map/textures/Material.001_baseColor.png");

	// grindstone
	//model = new Model("Assets/models/grindstone/scene.gltf");
	//texture = new Texture("Assets/models/grindstone/textures/Main_baseColor.jpeg");


	shaderProgram = new Shader("Source/Shaders/basic.vert", "Source/Shaders/basic.frag" );
	skyboxShader = new Shader("Source/Shaders/Skybox/SkyboxBasic.vert", "Source/Shaders/Skybox/SkyboxBasic.frag");


	Entity a;
	Entity entity = 1;
	EntityList.push_back(a);
	EntityList.push_back(entity);

	ModelComponentPool.Add(entity, ( model ));


	// tell the viewport
	glViewport(0, 0, WIDTH, HEIGHT);


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

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
	
	// use the shader program
	shaderProgram->Activate();
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	for (const auto& [entity, modelComponent] : ModelComponentPool.componentList) {
		modelComponent->model->Draw(*shaderProgram, camera);
	}
	shaderProgram->Unbind();
	
	skybox.Render(skyboxShader, camera.GetViewMat(), camera.GetProjMat(45.0f, 0.1f, 100.0f));

}



bool GraphicsSystem::Destroy()
{
	skybox.Destroy();
	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return true;
}


