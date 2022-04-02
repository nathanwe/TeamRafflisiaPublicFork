#ifndef _M_SYSTEM_H_
#define _M_SYSTEM_H_

#include "pch.h"
#include "../utils/SerializationHelper.h"

#include "Menu.h"


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
    void GoToPreviousMenu();

    void Draw();

    void ResetMenus();

    void ToggleDisplay();

    void AdjustForWindowSize();

    // Menu opening with Esc in release mode
    bool GetDisplay();
    void SetPrevMenu(int prevMenu);

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

    friend inline void to_json(ordered_json& j, MenuSystem& menuSys);
};


inline void from_json(const ordered_json& j, MenuSystem& menuSys)
{
    for (auto itr = j.begin(); itr != j.end(); itr = std::next(itr))
    {
        auto nMenu = menuSys.AddMenu(itr.key());
        from_json(itr.value(), *nMenu);
    }
}


inline void to_json(ordered_json& j, MenuSystem& menuSys)
{
    for (auto [name,menu] : menuSys.menus)
    {
        to_json(j[name],*menu);
    }
}




#endif
