#include "Shader.h"
#include "utils/Log.h"

// function to extract info from shader files
std::string GetFile(const char* filename) 
{
	std::ifstream in(filename, std::ios::binary);
	if (in) 
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}

// shader constructor, uses GetFile to find shader code
Shader::Shader(const char* vertexFile, const char* fragmentFile) 
{
	std::string vertexCode = GetFile(vertexFile);
	std::string fragmentCode = GetFile(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// create the two shaders
	LOG_INFO("Loading shader: {}", vertexFile);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");

	LOG_INFO("Loading shader: {}", fragmentFile);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");

	// create shader program and attach shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	CompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// use the shader program
void Shader::Activate() 
{
	glUseProgram(ID);
}

// deletes shader program
void Shader::Delete() 
{
	glDeleteProgram(ID);
}

// Debugging for the Shader class to make sure everything compiles nicely
void Shader::CompileErrors(unsigned int shader, const char* type) 
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") 
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) 
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER_COMPILATION_ERROR for: {0}", type);
		}
	}
	else 
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) 
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER_LINKING_ERROR: {0}", type);
		}
	}
}