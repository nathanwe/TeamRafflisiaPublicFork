#include "pch.h"

#include "MenuSystem.h"

#include "../Core/Shader.h"
#include "../Core/Engine.h"
#include "Menu.h"


extern Engine engine;

MenuSystem::MenuSystem()
{}

MenuSystem::~MenuSystem()
{
    for (auto [_, m] : menus)
        delete m;
    menus.clear();
}

void MenuSystem::Init()
{
    auto pMenu = this->AddMenu("PauseMenu");
    pMenu->AddButton(
        glm::vec2(0.1, 0.05), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f)
    );
    pMenu->AddButton(
        glm::vec2(0.1, 0.11), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f)
    );
    pMenu->AddButton(
        glm::vec2(0.1, 0.17), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f)
    );
    pMenu->AddButton(
        glm::vec2(0.1, 0.23), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f)
    );
    pMenu->Setup();

    menuShader = new Shader("Source/Shaders/MenuRenderer/UIMenuShader.shader");

    engine.CommandSys.GetCommand("Pause").SetActionToExecute(
        [&]()
        {
            display = !display;
		    engine.setPauseMenuMode(display);
            if (display)
                this->SetCurrentMenu("PauseMenu");
        }
    );
    Command& pCommand = engine.CommandSys.GetCommand("Pause");
    pCommand.keyboardcode = GLFW_KEY_P;
    pCommand.keyPressType = KeyPressType::Trigger;
}


Menu* MenuSystem::AddMenu(std::string menuName)
{
    if (menus.find(menuName) == menus.end())
        menus[menuName] = new Menu();
    return menus[menuName];
}

Menu* MenuSystem::GetMenu(std::string menuName)
{
    Menu* foundM = nullptr;
    if (menus.find(menuName) != menus.end())
        foundM = menus[menuName];
    return foundM;
}


bool MenuSystem::SetCurrentMenu(std::string menuName)
{
    if (menus.find(menuName) == menus.end())
        return false;
    currentMenu = menuName;
    return true;
}


void MenuSystem::Draw()
{
    if (display && !currentMenu.empty() && menus.find(currentMenu) != menus.end())
        menus[currentMenu]->Draw(*menuShader);
}



