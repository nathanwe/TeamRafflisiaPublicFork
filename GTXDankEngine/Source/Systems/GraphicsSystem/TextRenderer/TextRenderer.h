#ifndef _TEXT_RENDERER_
#define _TEXT_RENDERER_

#include "pch.h"


class Shader;

struct Character
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer
{
public:
    void Init(std::string fontPath);
    void RenderText(Shader& textShader, std::string text, float x, float y, float scale, glm::vec3 color);
public:
private:
private:
    std::map<unsigned char, Character> characters;
    
    unsigned int VAO, VBO;
};


#endif

