#pragma once

#include "pch.h"

std::string GetFile(const char* filename);

class Shader {
	public:
		unsigned int ID;
		Shader(const char* vertexFile, const char* fragmentFile);

		void Activate();
		void Delete();
	private:
		void CompileErrors(unsigned int shader, const char* type);
};

