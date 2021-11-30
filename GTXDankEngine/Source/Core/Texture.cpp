#include "pch.h"
#include "Texture.h"

#include "../utils/Log.h"


void Texture::OnLoad()
{
	if (!bytes)
	{
		LOG_ERROR("Loading texture {0} failed", debugPath);
	}

	// initialize texture
	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	if (numColCh == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColCh == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColCh == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	}
	else
	{
		LOG_ERROR("Loading texture failed, automatic Texture type recognition failed");
		throw std::invalid_argument("Automatic Texture type recognition failed");
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	LOG_INFO("Texture data loading ID {0}", ID);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Texture Constructor
Texture::Texture(const char* path)
{
	//LOG_INFO("Loading texture: {0}", path);
	debugPath = std::string(path);
	stbi_set_flip_vertically_on_load(true);
	bytes = stbi_load(path, &widthImg, &heightImg, &numColCh, 0);
	//if (stbi_failure_reason())
	//{
	//	LOG_ERROR("Texture load problem because {0}",stbi_failure_reason());
	//}
	//else
	//{
		LOG_INFO("Texture data loading succeed {0}", path);
	//}
	
}
Texture::Texture(std::string path) : Texture::Texture(path.c_str())
{
}
// Sets up uniform variable for shaders to read textures
void Texture::texUnit(Shader& shader, const char* uniform, unsigned int unit) 
{
	//GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	//shader.Activate();
	//glUniform1i(texUni, unit);
}

// Bind texture
void Texture::Bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

// Unbind texture
void Texture::Unbind() 
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
