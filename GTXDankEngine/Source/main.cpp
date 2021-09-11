#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include "Model.h"
#include "utils/Log.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;



// triangle vertices
//GLfloat vertices[] =
//{//                 COORDS                     //     COLORS       //
//	-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f, 1.0f, 0.0f, 0.0f, // lower left corner
//	 0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f, 0.0f, 1.0f, 0.0f, // lower right corner
//	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 0.0f, 0.0f, 1.0f, // upper corner
//	-0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f, 1.0f, 0.0f, 1.0f, // inner left
//	 0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f, 0.0f, 1.0f, 1.0f, // inner right
//	 0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f, 1.0f, 1.0f, 0.0f  // inner bottom
//};

// square vertices
//GLfloat vertices[] =
//{//         COORDS             //     COLORS      //   TEX COORDS
//	-0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // lower left corner
//	-0.5f,  0.5f, 0.0f,         0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // upper left corner
//	 0.5f,  0.5f, 0.0f,         0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // upper right corner
//	 0.5f, -0.5f, 0.0f,         1.0f, 1.0f, 0.0f,    1.0f, 0.0f  // lower right corner
//};

//// pyramid vertices
//GLfloat vertices[] =
//{//         COORDS             //     COLORS      //   TEX COORDS
//	-0.5f,  0.0f,  0.5f,         1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // lower left corner
//	-0.5f,  0.0f, -0.5f,         0.0f, 1.0f, 0.0f,    5.0f, 0.0f, // upper left corner
//	 0.5f,  0.0f, -0.5f,         0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // upper right corner
//	 0.5f,  0.0f,  0.5f,         1.0f, 1.0f, 0.0f,    5.0f, 0.0f, // lower right corner
//	 0.0f,  0.8f,  0.0f,         0.0f, 1.0f, 1.0f,    2.5f, 5.0f  // lower right corner
//};

// pyramid vertices with normals
//GLfloat vertices[] =
//{//         COORDS             //     COLORS      //   TEX COORDS //     NORMALS    //
//	-0.5f,  0.0f,  0.5f,         1.0f, 0.0f, 0.0f,    0.0f, 0.0f,   0.0f, -1.0f,  0.0f, // Bottom side
//	-0.5f,  0.0f, -0.5f,         0.0f, 1.0f, 0.0f,    0.0f, 5.0f,   0.0f, -1.0f,  0.0f, // Bottom side
//	 0.5f,  0.0f, -0.5f,         0.0f, 0.0f, 1.0f,    5.0f, 5.0f,   0.0f, -1.0f,  0.0f, // Bottom side
//	 0.5f,  0.0f,  0.5f,         1.0f, 1.0f, 0.0f,    5.0f, 0.0f,   0.0f, -1.0f,  0.0f, // Bottom side
//	///////////////////////////////////////////////////////////////////////////////////
//	-0.5f,  0.0f,  0.5f,         0.0f, 1.0f, 1.0f,    0.0f, 0.0f,  -0.8f,  0.5f,  0.0f, // Left side
//	-0.5f,  0.0f, -0.5f,         1.0f, 0.0f, 1.0f,    5.0f, 0.0f,  -0.8f,  0.5f,  0.0f, // Left side
//	 0.0f,  0.8f,  0.0f,         1.0f, 1.0f, 0.0f,    2.5f, 5.0f,  -0.8f,  0.5f,  0.0f, // Left side
//	///////////////////////////////////////////////////////////////////////////////////
//	-0.5f,  0.0f, -0.5f,         1.0f, 1.0f, 0.0f,    5.0f, 0.0f,   0.0f,  0.5f, -0.8f, // Back side
//	 0.5f,  0.0f, -0.5f,         1.0f, 0.0f, 1.0f,    0.0f, 0.0f,   0.0f,  0.5f, -0.8f, // Back side
//	 0.0f,  0.8f,  0.0f,         0.0f, 1.0f, 1.0f,    2.5f, 5.0f,   0.0f,  0.5f, -0.8f, // Back side
//	///////////////////////////////////////////////////////////////////////////////////
//	 0.5f,  0.0f, -0.5f,         0.0f, 1.0f, 1.0f,    0.0f, 0.0f,   0.8f,  0.5f,  0.0f, // Right side
//	 0.5f,  0.0f,  0.5f,         1.0f, 0.0f, 1.0f,    5.0f, 0.0f,   0.8f,  0.5f,  0.0f, // Right side
//	 0.0f,  0.8f,  0.0f,         1.0f, 1.0f, 0.0f,    2.5f, 5.0f,   0.8f,  0.5f,  0.0f, // Right side
//	///////////////////////////////////////////////////////////////////////////////////
//	 0.5f,  0.0f,  0.5f,         0.0f, 1.0f, 1.0f,    5.0f, 0.0f,   0.0f,  0.5f,  0.8f, // Front side
//	-0.5f,  0.0f,  0.5f,         1.0f, 0.0f, 1.0f,    0.0f, 0.0f,   0.0f,  0.5f,  0.8f, // Front side
//	 0.0f,  0.8f,  0.0f,         1.0f, 1.0f, 0.0f,    2.5f, 5.0f,   0.0f,  0.5f,  0.8f  // Front side
//};

// flat plane vertices
//GLfloat vertices[] =
//{ //    COORDINATES     //      COLORS      //   TEXCOORDS   //      NORMALS      //  
//	-1.0f, 0.0f,  1.0f,    0.0f, 0.0f, 0.0f,    0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
//	-1.0f, 0.0f, -1.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
//	 1.0f, 0.0f, -1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
//	 1.0f, 0.0f,  1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f
//};

// flat plane vertices using Vertex struct
//Vertex vertices[] = {
//	glm::vec3(-1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f),
//	glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f),
//	glm::vec3(1.0f, 0.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f),
//	glm::vec3(1.0f, 0.0f, 1.0f),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
//};

// index buffer to draw the Triforce
//GLuint indices[] =
//{
//	0, 3, 5, // Triforce of Wisdom
//	3, 2, 4, // Triforce of Courage
//	5, 4, 1 // Triforce of Power
//};

// index buffer to draw a square
//GLuint indices[] =
//{
//	0, 2, 1, // upper triangle
//	0, 3, 2  // lower triangle
//};

// index buffer to draw a pyramid
//GLuint indices[] =
//{
//	0, 1, 2,
//	0, 2, 3,
//	0, 1, 4,
//	1, 2, 4,
//	2, 3, 4,
//	3, 0, 4
//};

// index buffer to draw a pyramid with normals
//GLuint indices[] =
//{
//	0,  1,  2,  // Bottom
//	0,  2,  3,  // Bottom
//	4,  6,  5,  // Left
//	7,  9,  8,  // Back
//	10, 12, 11, // Right
//	13, 15, 14  // Front
//};

// index buffer for flat plane
//GLuint indices[] = {
//	0, 1, 2,
//	0, 2, 3
//};

// vertices for light cube
//GLfloat lightVertices[] = {
//	//     COORDS      //
//	-0.1f, -0.1f,  0.1f,
//	-0.1f, -0.1f, -0.1f,
//	 0.1f, -0.1f, -0.1f,
//	 0.1f, -0.1f,  0.1f,
//	-0.1f,  0.1f,  0.1f,
//	-0.1f,  0.1f, -0.1f,
//	 0.1f,  0.1f, -0.1f,
//	 0.1f,  0.1f,  0.1f
//};

// vertices for light cube using Vertex struct
Vertex lightVertices[] = {
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f,  0.1f,  0.1f)}
};

// indices for light cube
//GLuint lightIndices[] = {
//	0, 1, 2,
//	0, 2, 3,
//	0, 4, 7,
//	0, 7, 3,
//	3, 7, 6,
//	3, 6, 2,
//	2, 6, 5,
//	2, 5, 1,
//	1, 5, 4,
//	1, 4, 0,
//	4, 5, 6,
//	4, 6, 7
//};

int main() {
	// begin
	Log::init();

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
	if (pWindow == NULL) {
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

	Texture textures[]{
		Texture("Assets/Textures/planks.png", "diffuse", 0),
		Texture("Assets/Textures/planksSpec.png", "specular", 1)
	};


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

	//std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	//std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
	//std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));
	//Mesh floor(verts, inds, texs);

	// create lighting shaders
	//Shader lightShader("lighting.vert", "lighting.frag");

	//std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	//std::vector<GLuint> lightInds(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	//Mesh light(lightVerts, lightInds, texs);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	//glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::mat4 pyramidModel = glm::mat4(1.0f);
	//pyramidModel = glm::translate(pyramidModel, pyramidPos);

	//lightShader.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	//glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	
	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);

	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model("Assets/models/scroll/scene.gltf");

	// big loop
	while (!glfwWindowShouldClose(pWindow)) {
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
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		if (drawObject)
		{
			model.Draw(shaderProgram, camera);
		}

		//floor.Draw(shaderProgram, camera);
		//light.Draw(lightShader, camera);

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
	//lightShader.Delete();

	// end
	Log::shutdown();

	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return 0;
}