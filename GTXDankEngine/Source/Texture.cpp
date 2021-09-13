#include "Texture.h"
#include "utils/Log.h"

// Texture Constructor
Texture::Texture(const char* image, const char* texType, GLuint slot) {
	LOG_INFO("Loading texture: {0}", image);

	// Texture time
	type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	if (!bytes) LOG_ERROR("Loading texture failed");

	LOG_INFO("Texture data loaded", image);

	// initialize texture
	glGenTextures(1, &ID);
	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (numColCh == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColCh == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	}
	else if (numColCh == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	}
	else {
		LOG_ERROR("Loading texture failed, automatic Texture type recognition failed");
		throw std::invalid_argument("Automatic Texture type recognition failed");
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	LOG_INFO("Texture data loading succeed", image);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Sets up uniform variable for shaders to read textures
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

// Bind texture
void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

// Unbind texture
void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Delete texture
void Texture::Delete() {
	glDeleteTextures(1, &ID);
}