#ifndef SKYBOX_H
#define SKYOX_H



#include "pch.h"
#include "../Core/Shader.h"

/*
* Skybox object is neither a system nor component
* since it doesn't make sense that any entity has a skybox component
* For now, it's just an object lives inside Graphics system
*/

class Skybox
{
public:
	
	Skybox() = default;

	// delete copy and assignment constructor
	Skybox(const Skybox&) = delete;
	Skybox& operator= (const Skybox&) = delete;

	void Init();
	void Init(std::vector<std::string> &faces);

	void Render(glm::mat4 view, glm::mat4 proj, GLuint fbo);

	void Destroy();

	// this function may move to ResourceSystem
	unsigned int LoadCubemap(std::vector<std::string> faces);
	

private:
	unsigned int cubemapTexture;
	unsigned int skyboxVAO, skyboxVBO;

	Shader* shader;

};
#endif // !SKYBOX_H
