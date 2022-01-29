#ifndef _M_SYSTEM_H_
#define _M_SYSTEM_H_

#include "pch.h"

class Menu;
class Shader;

class MenuSystem
{
    
/// methods
public:
    MenuSystem();
    ~MenuSystem();

    void Init();

    Menu* AddMenu(std::string menuName);
    Menu* GetMenu(std::string menuName);

    bool SetCurrentMenu(std::string menuName);

    void Draw();

/// parameters
public:

/// methods
private:

/// parameters
private:
    bool display = false;
    std::map<std::string, Menu*> menus;
    std::string currentMenu;

    Shader* menuShader;

    /// WARNING!!!
    /// ACHTUNG!!!
    /// ВНИМАНИЕ!!!
    /// ALERTE!!!
    /// CURRENTLY NO MORE THAN TWO LEVELS OF MENUS!!!
    int prevMenu = -1;
    std::string menuStack[2];
};



#endif
