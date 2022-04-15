/*
    Written by Fedor Sharygin 2022
*/

#include "pch.h"

#include "MenuButton.h"

#include "../Core/Shader.h"
#include "../Core/Texture.h"

/// used to get WIDTH and HEIGHT
#include "../Systems/GraphicsSystem/GraphicsSystem.h"


#include "../Core/Engine.h"

/// Engine is used to access the window, camera and the GraphicsSystem
extern Engine engine;

std::function<void ()> buttonBackCommand = [&]()
{
    engine.GraphicsSys.GetMenuSystem().GoToPreviousMenu();
};
std::function<void (std::string)> buttonNextCommand = [&](std::string nextMenu)
{
    engine.GraphicsSys.GetMenuSystem().SetCurrentMenu(nextMenu);
};


MenuButton::MenuButton()
    :
relativePos(std::make_pair(false, false)), position(glm::vec2(0,0)),
relativeDimns(std::make_pair(false, false)), dimensions(glm::vec2(0,0)),
rgbTint(0,0,0,1),
/// 2D position     RGB color       Texture coordinates
quadVerticesColorTexture((2 + 4 + 2) * 4),
indices({ 0,1,2, 1,2,3 })
{
    SetVertices();
}

MenuButton::MenuButton(glm::vec2 nPos, glm::vec2 nDimensions, glm::vec4 nRGBTint,
        std::pair<bool, bool> nRelativePos,
        std::pair<bool, bool> nRelativeDimns)
    :
relativePos(nRelativePos), position(nPos),
relativeDimns(nRelativeDimns), dimensions(nDimensions),
rgbTint(nRGBTint),
/// 2D position     RGB color       Texture coordinates
quadVerticesColorTexture((2 + 4 + 2) * 4),
indices({ 0,1,2, 1,2,3 })
{
    SetVertices();
}


MenuButton* MenuButton::SetPosition(glm::vec2 nPos, std::pair<bool, bool> nRelativePos)
{
    position = nPos;
    relativePos = nRelativePos;
    SetVertices();
    return this;
}

MenuButton* MenuButton::SetDimensions(glm::vec2 nDimensions, std::pair<bool, bool> nRelativeDimns)
{
    dimensions = nDimensions;
    relativeDimns = nRelativeDimns;
    SetVertices();
    return this;
}

MenuButton* MenuButton::SetColor(glm::vec4 nRGBTint)
{
    rgbTint.x = nRGBTint.x / 255.0f;
    rgbTint.y = nRGBTint.y / 255.0f;
    rgbTint.z = nRGBTint.z / 255.0f;
    rgbTint.a = nRGBTint.a;
    SetVertices();
    return this;
}


void MenuButton::Draw(Shader& shader)
{
    //shader.setMat4("orthoMat", glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), 0.1f, 100.0f));

    double xMPos, yMPos;
    glfwGetCursorPos(engine.window, &xMPos, &yMPos);
    if (active)
    {
        if (std::abs(xMPos - xPos) <= btnWidth/2.0f && std::abs(yMPos - yPos) <= btnHeight/2.0f)
        {
            if (engine.InputSys.IsLeftMousePressed())
            {
                shader.setVec3("shade", glm::vec3(0.6f,0.6f,0.6f));
                readyToExecute = true;
            }
            else
            {
                shader.setVec3("shade", glm::vec3(0.93f, 0.93f, 0.93f));
                if (readyToExecute)
                {
                    readyToExecute = false;
                    this->Execute();
                }
            }
        }
        else
        {
            shader.setVec3("shade", glm::vec3(0.82f,0.82f,0.82f));
            readyToExecute = false;
        }
    }
    else
    {
        shader.setVec3("shade", glm::vec3(0.6f,0.6f,0.6f));
    }

    shader.setInt("haveTxtr", haveTexture);
    if (haveTexture)
    {
        auto bTxtr = TextureResourceManger.GetResourceHandle(buttonTexture);
        shader.setTexture("txtr", bTxtr->GetPointer()->GetID());
    }

    // draw button
    shader.Bind();
    glBindVertexArray(VAO);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    shader.unBind();
}


void MenuButton::SetVertices()
{
    xPos        = relativePos.first    ? position.x   * float(engine.GraphicsSys.camera.width)   : position.x;
    yPos        = relativePos.second   ? position.y   * float(engine.GraphicsSys.camera.height)  : position.y;
    btnWidth    = relativeDimns.first  ? dimensions.x * float(engine.GraphicsSys.camera.width)   : dimensions.x;
    btnHeight   = relativeDimns.second ? dimensions.y * float(engine.GraphicsSys.camera.height)  : dimensions.y;

    for (int i = 0; i < 4; ++i)
    {
        int startIdx = (2 + 4 + 2) * i;
        /// set position
        /// since window goes from bottom to top [-1,1] and we work from top to bottom
        /// y-value must be flipped to reflect the correct orientation
        quadVerticesColorTexture[startIdx]    =  (xPos + btnWidth  / 2 * (i % 2 == 0 ? -1.0f : 1.0f) - float(engine.GraphicsSys.camera.width)  / 2.0f) / (float(engine.GraphicsSys.camera.width)  / 2.0f);
        quadVerticesColorTexture[startIdx+1]  = -(yPos + btnHeight / 2 * (i / 2 == 0 ? -1.0f : 1.0f) - float(engine.GraphicsSys.camera.height) / 2.0f) / (float(engine.GraphicsSys.camera.height) / 2.0f);
        /// set color
        quadVerticesColorTexture[startIdx+2]  = rgbTint.r;
        quadVerticesColorTexture[startIdx+3]  = rgbTint.g;
        quadVerticesColorTexture[startIdx+4]  = rgbTint.b;
        quadVerticesColorTexture[startIdx+5]  = rgbTint.a;
        /// set texture coordinates
        quadVerticesColorTexture[startIdx+6]  = i % 2;
        quadVerticesColorTexture[startIdx+7]  = i / 2 == 0;
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
    glBufferData(GL_ARRAY_BUFFER, quadVerticesColorTexture.size() * sizeof(float), quadVerticesColorTexture.data(), GL_STATIC_DRAW);



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


MenuButton* MenuButton::SetActionToExecute(std::function<void()> actionToExecute)
{
	this->actionToExecute = actionToExecute;
    return this;
}

void MenuButton::Execute()
{
    if (actionToExecute)
        this->actionToExecute();
}


void MenuButton::LoadLevel()
{
    engine.SceneSys.LoadScene(levelToLoad);
}



MenuButton* MenuButton::SetTexture(std::string texturePath)
{
    haveTexture = true;
    buttonTexture = texturePath;
    return this;
}



const glm::vec2& MenuButton::GetPosition() const
{
    return position;
}

const glm::vec2& MenuButton::GetDimensions() const
{
    return dimensions;
}



/// JSON functionality
/// Since inline functions must be defined in the header
/// but engine cannot be used there, I must go around it
/// by defining separate json functions to use engine

void button_from_json(const ordered_json& j, MenuButton& menuBut)
{
    bool changed = false;
    if (j.find("Texture") != j.end())
    {
        menuBut.SetTexture(j["Texture"]);
        changed = true;
    }
    else
    if (j.find("Color") != j.end())
    {
        glm::vec4 nColor;
        from_json(j["Color"], nColor);
        menuBut.SetColor(nColor);
        changed = true;
    }
    else
    {
        changed = true;
        menuBut.SetTexture("Assets/Textures/ButtonBackground.png");
        menuBut.named = true;
    }

    if (changed)
    {
        menuBut.SetVertices();
    }

    if (j.find("Command") != j.end())
    {
        std::string command;
        from_json(j["Command"], command);

        if (command.compare("Back") == 0)
        {
            menuBut.SetActionToExecute(buttonBackCommand);
        }
        /// Add new menus
        else if (command.compare("Continue") == 0)
        {
            menuBut.SetActionToExecute([&](){
                engine.GraphicsSys.GetMenuSystem().ResetMenus();
            });
        }
        else if (command.compare("Exit") == 0)
        {
            menuBut.SetActionToExecute([&](){
                glfwSetWindowShouldClose(engine.window, true);
            });
        }
        else if (command.compare("Start") == 0)
        {
            menuBut.SetActionToExecute([&](){
                engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
                engine.SceneSys.LoadScene(1);
            });
        }
        else if (command.compare("Next level") == 0)
        {
            menuBut.SetActionToExecute([&](){
                engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
                engine.SceneSys.LoadNextLevel();
            });
        }
        else if (command.compare("Prev level") == 0)
        {
            menuBut.SetActionToExecute([&](){
                engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
                engine.SceneSys.LoadPreviousLevel();
            });
        }
        else if (command.compare("Restart") == 0)
        {
            menuBut.SetActionToExecute([&](){
                engine.SceneSys.LoadScene(engine.SceneSys.GetCurrentLevel());
                engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
            });
        }
        else if (command.compare("Credits") == 0)
        {
            menuBut.SetActionToExecute([&](){
                engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
                engine.SceneSys.LoadScene(engine.SceneSys.GetCreditsLevel());
            });
        }
        /// in this case the command is the name of the next level
        else if (command.substr(0, 6).compare("LEVEL ") == 0)
        {
            menuBut.levelToLoad = stoi(command.substr(6));
            menuBut.SetActionToExecute([&](){
                engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
                menuBut.LoadLevel();
            });
        }
        /// in this case command is the name of the next menu
        else
        {
            menuBut.nMenu = command;
            std::function<void ()> nextMenu = [&]()
            {
                engine.GraphicsSys.GetMenuSystem().SetCurrentMenu(menuBut.nMenu);
            };
            menuBut.SetActionToExecute(nextMenu);
        }
    }
}

void button_to_json(ordered_json& j, MenuButton& menuBut)
{
    if (menuBut.haveTexture)
    {
        to_json(j["Texture"],menuBut.buttonTexture);
    }
    else
    {
        to_json(j["Color"],menuBut.rgbTint);
    }
}


void MenuButton::AdjustForWindowSize()
{
    SetVertices();
    Setup();
}

