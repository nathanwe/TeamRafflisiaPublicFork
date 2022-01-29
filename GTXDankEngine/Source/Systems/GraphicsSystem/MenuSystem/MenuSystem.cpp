#include "pch.h"

#include "MenuSystem.h"

#include "../Core/Shader.h"
#include "../Core/Engine.h"
#include "Menu.h"
#include "MenuButton.h"


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
    /// Add pause menu
    auto pMenu = this->AddMenu("PauseMenu");
    
    pMenu->AddButton("Continue",
        glm::vec2(0.1, 0.05), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute([&](){
            display = !display;
		    engine.setPauseMenuMode(display);
            --prevMenu;
            glfwSetCursorPos(engine.window, (WIDTH / 2), (HEIGHT / 2));
    })
    ->SetTexture("Assets/Textures/Continue_test.png");
    
    pMenu->AddButton("Test1",
        glm::vec2(0.1, 0.11), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute([&](){
        this->SetCurrentMenu("OptionsMenu1");
    })
    ->SetTexture("Assets/Textures/Options_test.png");
    
    pMenu->AddButton("Test2",
        glm::vec2(0.1, 0.17), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute([&](){
        this->SetCurrentMenu("OptionsMenu2");
    });
    
    pMenu->AddButton("Exit",
        glm::vec2(0.1, 0.23), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute([&](){
        glfwSetWindowShouldClose(engine.window, true);
    })
    ->SetTexture("Assets/Textures/Exit_test.png");
    
    pMenu->Setup();


    /// Back command button
    auto backCommand = [&]() {
        currentMenu = menuStack[--prevMenu];
    };

    /// Something is wrong with Options Menus!!!
    /// Add Options Menu 1
    auto oMenu1 = this->AddMenu("OptionsMenu1");
    oMenu1->AddButton("Back",
        glm::vec2(0.1, 0.05), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute(backCommand)
    ->SetTexture("Assets/Textures/Exit_test.png");
    oMenu1->Setup();

    /// Add Options Menu 2
    auto oMenu2 = this->AddMenu("OptionsMenu2");
    oMenu2->AddButton("DeeperMenu",
        glm::vec2(0.1, 0.05), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute([&](){
        this->SetCurrentMenu("OptionsMenu2Menu");
    })
    ->SetTexture("Assets/Textures/Options_test.png");

    oMenu2->AddButton("Back",
        glm::vec2(0.1, 0.11), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute(backCommand)
    ->SetTexture("Assets/Textures/Exit_test.png");
    oMenu2->Setup();

    /// Add Options Menu 2 Menu
    auto oMenu2Menu = this->AddMenu("OptionsMenu2Menu");
    oMenu2Menu->AddButton("Back",
        glm::vec2(0.1, 0.05), std::make_pair(true, true),
        glm::vec2(0.1, 0.04), std::make_pair(true, true),
        glm::vec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f)
    )
    ->SetActionToExecute(backCommand)
    ->SetTexture("Assets/Textures/Exit_test.png");
    oMenu2Menu->Setup();

    menuShader = new Shader("Source/Shaders/MenuRenderer/UIMenuShader.shader");

    engine.CommandSys.GetCommand("Pause").SetActionToExecute(
        [&]()
        {
            display = !display;
		    engine.setPauseMenuMode(display);
            if (display)
                this->SetCurrentMenu("PauseMenu");
            else
            {
                prevMenu = -1;
                glfwSetCursorPos(engine.window, (WIDTH / 2), (HEIGHT / 2));
            }
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
    /// WARNING!!!
    /// ACHTUNG!!!
    /// ВНИМАНИЕ!!!
    /// ALERTE!!!
    /// CURRENTLY NO MORE THAN TWO LEVELS OF MENUS!!!

    if (menus.find(menuName) == menus.end())
        return false;
    
    if (prevMenu != -1)
        menuStack[prevMenu++] = currentMenu;
    else
        ++prevMenu;
    
    currentMenu = menuName;
    return true;
}


void MenuSystem::Draw()
{
    if (display && !currentMenu.empty() && menus.find(currentMenu) != menus.end())
    {
        bool isom1 = currentMenu.compare("OptionsMenu1") == 0;
        menus[currentMenu]->Draw(*menuShader);
    }
}



