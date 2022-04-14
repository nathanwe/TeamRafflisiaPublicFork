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
            if (engine.SceneSys.GetCurrentLevel() > -1)
            {
                ToggleDisplay();
                if (display)
                    this->SetCurrentMenu("Pause");
                else
                {
                    prevMenu = -1;
                    glfwSetCursorPos(engine.window, (engine.GraphicsSys.camera.width / 2), (engine.GraphicsSys.camera.height / 2));
                }
            }
        }
    );
    Command& pCommand = engine.CommandSys.GetCommand("Pause");
    pCommand.keyboardcode = GLFW_KEY_ESCAPE;
    pCommand.gamepadCode = 7;
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
        LOG_WARN("Trying to view unknown menu {0}", menuName);
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
    if (!currentMenu.empty() && menus.find(currentMenu) != menus.end() && display)
    {
        if (currentMenu.compare("ExitMenu") == 0)
        {
            engine.GraphicsSys.DrawCustomText("ARE YOU SURE", 2.3f, glm::vec2(100, 300), glm::vec3(237.0f,28.0f,36.0f)/255.0f);
        }
        else if (currentMenu.compare("B2MMenu") == 0)
        {
            engine.GraphicsSys.DrawCustomText("ARE YOU SURE", 2.3f, glm::vec2(100,200), glm::vec3(237.0f,28.0f,36.0f)/255.0f);
            engine.GraphicsSys.DrawCustomText("BACK TO MAIN MENU", 1.8f, glm::vec2(23,350), glm::vec3(237.0f,28.0f,36.0f)/255.0f);
            
        }
        menus[currentMenu]->Draw(*menuShader);
    }
}


void MenuSystem::ResetMenus()
{
    display = !display;
    prevMenu = -1;
	engine.setPauseMenuMode(display);
    glfwSetCursorPos(engine.window, (engine.GraphicsSys.camera.width / 2), (engine.GraphicsSys.camera.height / 2));
}


void MenuSystem::ToggleDisplay()
{
    display = !display;
    prevMenu = -1;
    engine.setPauseMenuMode(display);
    engine.GraphicsSys.drawCredits = false;
}


void MenuSystem::AdjustForWindowSize()
{
    for (auto [_, m] : menus)
        m->AdjustForWindowSize();
}


// Menu opening with Esc in release mode
bool MenuSystem::GetDisplay()
{
    return this->display;
}
void MenuSystem::SetPrevMenu(int prevMenu)
{
    this->prevMenu = prevMenu;
}