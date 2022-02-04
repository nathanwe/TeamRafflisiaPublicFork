#include "pch.h"

#include "Menu.h"
#include "../Core/Shader.h"

/// used to get WIDTH and HEIGHT
#include "../Systems/GraphicsSystem/GraphicsSystem.h"


Menu::Menu()
    :
buttons(std::map<std::string, MenuButton*>()),
vertices(std::vector<int>(4 * (2 + 4 + 2))), indices({0,1,2,1,2,3})
{
    for (int i = 0; i < 4; ++i)
    {
        vertices[8*i]   = (i % 2 == 0 ? 0 : WIDTH );
        vertices[8*i+1] = (i / 2 == 0 ? 0 : HEIGHT);
    }
}

Menu::~Menu()
{
    for (auto [_,mb] : buttons)
        delete mb;
    
    buttons.clear();
}


MenuButton* Menu::AddButton(std::string name,
    glm::vec2 position,   std::pair<bool, bool> isPosRelative,
    glm::vec2 dimensions, std::pair<bool, bool> isDimRelative,
    glm::vec4 rgba)
{
    if (buttons.find(name) == buttons.end())
        buttons[name] = new MenuButton(position, dimensions, rgba, isPosRelative, isDimRelative);
    else
        LOG_WARN("Attempt to override an existing button {0}", name);
    return buttons[name];
}

MenuButton* Menu::GetButton(std::string name)
{
    MenuButton* fButton = nullptr;
    if (buttons.find(name) == buttons.end())
    {
        LOG_WARN("No button with name {0}", name);
    }
    else
    {
        fButton = buttons[name];
    }
    return fButton;
}

void Menu::Setup()
{
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int IBO, VBO;
    // VBO
    //VertexBuffer vbo(&quadVerticesColorTexture[0], quadVerticesColorTexture.size() * sizeof(Vertex));
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(int) * (2 + 4 + 2), vertices.data(), GL_STATIC_DRAW);


    // IBO
    //IndexBuffer ibo(&indices[0], indices.size());
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(int), (void*)0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(int), (void*)2);
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(int), (void*)6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    /// setup all of the buttons
    for (auto& [_,mb] : buttons)
        mb->Setup();
}

void Menu::Draw(Shader& shader)
{
/*    shader.Bind();
    glBindVertexArray(VAO);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    shader.unBind();*/

    for (auto& [_,mb] : buttons)
    {
        bool isTest2 = _.compare("Test2") == 0;
        mb->Draw(shader);
    }
}

