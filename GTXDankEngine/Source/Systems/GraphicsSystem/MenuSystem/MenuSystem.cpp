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
    menuShader = new Shader("Source/Shaders/MenuRenderer/UIMenuShader.shader");
    auto* handle = SerializationResourceManager.GetResourceHandleNoThread(std::string("Assets/MenuSystemFiles/BasicMenuSystem.json"));
    ordered_json archetypeJson = handle->GetPointer()->data;
    from_json(archetypeJson, *this);
    for (auto& [_,menu] : menus)
        menu->Setup();

    engine.CommandSys.GetCommand("Pause").SetActionToExecute(
        [&]()
        {
            display = !display;
		    engine.setPauseMenuMode(display);
            if (display)
                this->SetCurrentMenu("Pause");
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
    {
        LOG_WARN("Trying to view unknonw menu {0}", menuName);
        return false;
    }
    
    if (prevMenu != -1)
        menuStack[prevMenu++] = currentMenu;
    else
        ++prevMenu;
    
    currentMenu = menuName;
    return true;
}

void MenuSystem::GoToPreviousMenu()
{
    currentMenu = menuStack[--prevMenu];
}


void MenuSystem::Draw()
{
    if (display && !currentMenu.empty() && menus.find(currentMenu) != menus.end())
    {
        bool isom1 = currentMenu.compare("OptionsMenu1") == 0;
        menus[currentMenu]->Draw(*menuShader);
    }
}


void MenuSystem::ResetMenus()
{
    display = !display;
    prevMenu = -1;
	engine.setPauseMenuMode(display);
    glfwSetCursorPos(engine.window, (WIDTH / 2), (HEIGHT / 2));
}


