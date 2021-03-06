#ifndef TEXTURE_H
#define TEXTURE_H

#include "pch.h"
#include "Shader.h"

class Texture {
	private:
		GLuint ID;

	public:

		void OnLoad();

		Texture(const char* path);
		Texture(std::string path);
		inline ~Texture() { glDeleteTextures(1, &ID); }

		void texUnit(Shader& shader, const char* uniform, unsigned int unit);
		void Bind(unsigned int unit);
		void Unbind();
		inline GLuint GetID() const { return ID; };

private:
	//temp but its easyer then passing them around
		unsigned char* bytes;
		int widthImg, heightImg, numColCh;
		std::string debugPath;

};
#endif // !TEXTURE_H
