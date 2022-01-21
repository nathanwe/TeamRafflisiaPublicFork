/*
    Written by Fedor Sharygin 2022
*/

#include "pch.h"

#include "MenuButton.h"

#include "../Core/Engine.h"
#include "../Core/Shader.h"
#include "../GraphicsSystem/Quad.h"

/// used to get WIDTH and HEIGHT
#include "../GraphicsSystem/GraphicsSystem.h"


/// Engine is used to access the window and camera
extern Engine engine;


MenuButton::MenuButton()
    :
relativePos(std::make_pair(false, false)), position(glm::vec2(0,0)),
relativeDimns(std::make_pair(false, false)), dimensions(glm::vec2(0,0)),
rgbTint(0,0,0,1),
/// 2D position     RGB color       Texture coordinates
quadVerticesColorTexture((2 + 4 + 2) * 4),
indices({ 0,1,2, 1,2,3 }),
mQuad(new Quad())
{}

MenuButton::MenuButton(glm::vec2 nPos, glm::vec2 nDimensions, glm::vec4 nRGBTint,
        std::pair<bool, bool> nRelativePos,
        std::pair<bool, bool> nRelativeDimns)
    :
relativePos(nRelativePos), position(nPos),
relativeDimns(nRelativeDimns), dimensions(nDimensions),
rgbTint(nRGBTint),
/// 2D position     RGB color       Texture coordinates
quadVerticesColorTexture((2 + 4 + 2) * 4),
indices({ 0,1,2, 1,2,3 }),
mQuad(new Quad())
{
    SetVertices();
}


void MenuButton::SetPosition(glm::vec2 nPos, std::pair<bool, bool> nRelativePos)
{
    position = nPos;
    relativePos = nRelativePos;
    SetVertices();
}

void MenuButton::SetDimensions(glm::vec2 nDimensions, std::pair<bool, bool> nRelativeDimns)
{
    dimensions = nDimensions;
    relativeDimns = nRelativeDimns;
    SetVertices();
}

void MenuButton::SetColor(glm::vec4 nRGBTint)
{
    rgbTint = nRGBTint;
}


void MenuButton::Draw(Shader& shader)
{
    // draw button
/*    shader.Bind();
    glBindVertexArray(VAO);
    glDisable(GL_DEPTH_TEST);
    shader.setMat4("orthoMat", glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), 0.1f, 100.0f));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    shader.unBind();*/

    mQuad->Draw(shader);
}


void MenuButton::SetVertices()
{
    float xPos        = relativePos.first    ? position.x   * float(WIDTH)   : position.x;
    float yPos        = relativePos.second   ? position.y   * float(HEIGHT)  : position.y;
    float btnWidth    = relativeDimns.first  ? dimensions.x * float(WIDTH)   : dimensions.x;
    float btnHeight   = relativeDimns.second ? dimensions.y * float(HEIGHT)  : dimensions.y;

    for (int i = 0; i < 4; ++i)
    {
        int startIdx = (2 + 4 + 2) * i;
        /// set position
        quadVerticesColorTexture[startIdx]    = (xPos + btnWidth  / 2 * (i % 2 == 0 ? -1.0f : 1.0f) - float(WIDTH)  / 2.0f) / (float(WIDTH)  / 2.0f);
        quadVerticesColorTexture[startIdx+1]  = (yPos + btnHeight / 2 * (i / 2 == 0 ? -1.0f : 1.0f) - float(HEIGHT) / 2.0f) / (float(HEIGHT) / 2.0f);
        /// set color
        quadVerticesColorTexture[startIdx+2]  = rgbTint.r;
        quadVerticesColorTexture[startIdx+3]  = rgbTint.g;
        quadVerticesColorTexture[startIdx+4]  = rgbTint.b;
        quadVerticesColorTexture[startIdx+5]  = rgbTint.a;
        /// set texture coordinates
        quadVerticesColorTexture[startIdx+6]  = i % 2;
        quadVerticesColorTexture[startIdx+7]  = i / 2;
    }
}

void MenuButton::Setup()
{
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int IBO, VBO;
    // VBO
    //VertexBuffer vbo(&quadVerticesColorTexture[0], quadVerticesColorTexture.size() * sizeof(Vertex));
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, quadVerticesColorTexture.size() * sizeof(int) * (2 + 4 + 2), quadVerticesColorTexture.data(), GL_STATIC_DRAW);



    // IBO
    //IndexBuffer ibo(&indices[0], indices.size());
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);



    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

