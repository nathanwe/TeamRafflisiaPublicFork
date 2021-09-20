#pragma once
#include "pch.h"


class EBO {
	public:
		GLuint ID;
		EBO(std::vector<GLuint>& indices);

		void Bind();
		void Unbind();
		void Delete();
};
