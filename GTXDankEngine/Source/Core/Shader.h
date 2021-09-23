#ifndef SHADER_H
#define SHADER_H




#include "pch.h"

std::string GetFile(const char* filename);

class Shader {
	public:
		unsigned int ID;
		Shader(const char* vertexFile, const char* fragmentFile);

		void Activate();
		inline void Unbind() { glUseProgram(0); }
		void Delete();

		inline int getUniformLocation(const char* name)
		{
			return glGetUniformLocation(ID, name);
		}

	private:
		void CompileErrors(unsigned int shader, const char* type);
};

#endif
