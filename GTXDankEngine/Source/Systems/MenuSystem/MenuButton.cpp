/*
    Written by Fedor Sharygin 2022
*/

#include <MenuButton.h>

#include <Engine.h>
#include <Shader.h>

/// used to get WIDTH and HEIGHT
#include <GraphicsSystem/GraphicsSystem.h>


/// Engine is used to access the window and camera
extern Engine engine;


MenuButton::MenuButton()
    :
relativePos(std::make_pair(false, false)), position(glm::vec2(0,0)),
relativeDimns(std::make_pair(false, false)), dimensions(glm::vec2(0,0)),
rgbTint(0,0,0),
/// 2D position     RGB color       Texture coordinates
quadVerticesColorTexture((2 + 3 + 2) * 4),
indices(4)
{}

MenuButton::MenuButton(glm::vec2 nPos, glm::vec2 nDimensions, glm::vec3 nRGBTint,
        std::pair<bool, bool> nRelativePos,
        std::pair<bool, bool> nRelativeDimns)
    :
relativePos(nRelativePos), position(nPos),
relativeDimns(nRelativeDimns), dimensions(nDimensions),
rgbTint(nRGBTint),
/// 2D position     RGB color       Texture coordinates
quadVerticesColorTexture((2 + 3 + 2) * 4),
indices(4)
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


void MenuButton::Draw(Shader& shader)
{
    // draw button
    shader.Bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.unBind();
}


void MenuButton::SetVertices()
{
    int xPos        = relativePos.first    ? position.x   * float(WIDTH)   : position.x;
    int yPos        = relativePos.second   ? position.y   * float(HEIGHT)  : position.y;
    int btnWidth    = relativeDimns.first  ? dimensions.x * float(WIDTH)   : dimensions.x;
    int btnHeight   = relativeDimns.second ? dimensions.y * float(HEIGHT)  : dimensions.y;

    for (int i = 0; i < 4; ++i)
    {
        int startIdx = (2 + 3 + 2) * i;
        /// set position
        quadVerticesColorTexture[startIdx]    = xPos + btnWidth  / 2 * (i % 2 == 0 ? -1 : 1);
        quadVerticesColorTexture[startIdx+1]  = yPos + btnHeight / 2 * (i / 2 == 0 ? -1 : 1);
        /// set color
        quadVerticesColorTexture[startIdx+2]  = rgbTint.r;
        quadVerticesColorTexture[startIdx+3]  = rgbTint.g;
        quadVerticesColorTexture[startIdx+4]  = rgbTint.b;
        /// set texture coordinates
        quadVerticesColorTexture[startIdx+5]  = i % 2;
        quadVerticesColorTexture[startIdx+6]  = i / 2;

        indices[i] = i;
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
    glBufferData(GL_ARRAY_BUFFER, quadVerticesColorTexture.size() * sizeof(Vertex), quadVerticesColorTexture.data(), GL_STATIC_DRAW);



    // IBO
    //IndexBuffer ibo(&indices[0], indices.size());
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);



    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(int), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(int), (void*)2);
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(int), (void*)5);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

