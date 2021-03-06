#include "pch.h"

#include "Menu.h"
#include "../Core/Shader.h"
#include "../Core/Engine.h"

/// used to get WIDTH and HEIGHT
#include "../Systems/GraphicsSystem/GraphicsSystem.h"

extern Engine engine;

Menu::Menu()
    :
buttons(std::map<std::string, MenuButton*>()),
vertices(std::vector<float>(4 * (2 + 2))), indices({ 0,1,2, 1,2,3 })
{
    /// for slider testing
    //sliders["basic"] = new MenuSlider("BGM Volume", glm::vec2(400, 700), glm::vec2(600, 15));
    //sliders["basic"]->SetDivision(10);
}

Menu::~Menu()
{
    for (auto [_,mb] : buttons)
        delete mb;
    
    buttons.clear();
}


void Menu::DeactivateLevelButtons(int prevLevel)
{
    prevLevel++;
    for (; prevLevel != buttons.size(); ++prevLevel)
    {
        buttons["LEVEL " + std::to_string(prevLevel)]->active = false;
    }
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

MenuSlider* Menu::AddSlider(std::string name,
    glm::vec2 position, glm::vec2 dimensions)
{
    if (sliders.find(name) == sliders.end())
        sliders[name] = new MenuSlider(name, position, dimensions);
    else
        LOG_WARN("Attempt to override an existing slider {0}", name);
    return sliders[name];
}

MenuSlider* Menu::GetSlider(std::string name)
{
    MenuSlider* fSlider = nullptr;
    if (sliders.find(name) == sliders.end())
    {
        LOG_WARN("No slider with name {0}", name);
    }
    else
    {
        fSlider = sliders[name];
    }
    return fSlider;
}


void Menu::Setup()
{
    /// Setup vertices for background
    for (int i = 0; i < 4; ++i)
    {
        vertices[4*i]   = (i % 2? 1 : -1);
        vertices[4*i+1] = (i / 2? 1 : -1);
        vertices[4*i+2] =  i % 2;
        vertices[4*i+3] =  i / 2;
    }

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int IBO, VBO;
    // VBO
    //VertexBuffer vbo(&quadVerticesColorTexture[0], quadVerticesColorTexture.size() * sizeof(Vertex));
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * (2 + 2), vertices.data(), GL_STATIC_DRAW);


    // IBO
    //IndexBuffer ibo(&indices[0], indices.size());
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    /// setup all of the buttons
    for (auto& [_,mb] : buttons)
        mb->Setup();

    /// setup all of the sliders
    for (auto& [_,ms] : sliders)
        ms->Setup();
}


void Menu::SetTexture(std::string background)
{
    haveBackground = true;
    texture = background;
}


void Menu::Draw(Shader& shader)
{
    if (haveBackground)
    {
        /// if we have a background set the shader parameters
        shader.setInt("haveTxtr", haveBackground);
        auto mTxtr = TextureResourceManger.GetResourceHandle(texture);
        shader.setTexture("txtr", mTxtr->GetPointer()->GetID());
        shader.setVec3("shade", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.Bind();
        glBindVertexArray(VAO);
        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);
        shader.unBind();
    }

    /// draw all of the buttons
    for (auto& [name, mb] : buttons)
    {
        mb->Draw(shader);
        /// if named => draw name over button
        if (mb->named)
        {
            const glm::vec2& pos  = mb->GetPosition();
            const glm::vec2& dims = mb->GetDimensions();
            glm::vec2 preCalc = pos-dims/2.0f + glm::vec2(10,40);
            engine.GraphicsSys.DrawCustomText(name, 0.43f, preCalc, glm::vec3(0));
        }
    }

    
    /// draw all of the sliders
    for (auto& [_,ms] : sliders)
        ms->Draw();
}


void Menu::AdjustForWindowSize()
{
    for (auto& [_,mb] : buttons)
    {
        mb->AdjustForWindowSize();
    }

    for (auto& [_,ms] : sliders)
    {
        ms->AdjustForWindowSize();
    }
}

