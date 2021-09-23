#ifndef TEXTURE_H
#define TEXTURE_H



#include "Shader.h"

#include <glad/glad.h>

class Texture {
	private:
		GLuint ID;

	public:

		Texture(const char* path);
		inline ~Texture() { glDeleteTextures(1, &ID); }

		void texUnit(Shader& shader, const char* uniform, unsigned int unit);
		void Bind(unsigned int unit);
		void Unbind();
		
};
#endif // !TEXTURE_H
