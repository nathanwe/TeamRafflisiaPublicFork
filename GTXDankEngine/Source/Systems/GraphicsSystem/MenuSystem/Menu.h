/*
    Written by Fedor Sharygin 2022
*/

#ifndef _UI_MENU_
#define _UI_MENU_

#include "pch.h"
#include "../utils/SerializationHelper.h"

#include "MenuButton.h"


class Shader;

class Menu
{
    
/// methods
public:
    Menu();
    ~Menu();

    MenuButton* AddButton(std::string name,
        glm::vec2 position,   std::pair<bool, bool> isPosRelative,
        glm::vec2 dimensions, std::pair<bool, bool> isDimRelative,
        glm::vec4 rgba);
    
    MenuButton* GetButton(std::string name);

    void Draw(Shader& shader);
    void Setup();

    void SetTexture(std::string background);

/// parameters
public:

    bool isMain = false;
    
/// methods
private:

/// parameters
private:
    std::map<std::string, MenuButton*> buttons;
    
    /// values for drawing the menu background
    std::vector<float> vertices;
	unsigned int VAO;
    std::vector<unsigned int> indices;
    bool haveBackground = false;
    std::string texture;
    
    friend inline void to_json(ordered_json& j, Menu& menu);
};


inline void from_json(const ordered_json& j, Menu& menu)
{
    glm::vec2 commonDmns, startPnt;
    from_json(j["Size"], commonDmns);
    from_json(j["First"], startPnt);

    if (j.find("Background") != j.end())
    {
        std::string txtrPath;
        from_json(j["Background"], txtrPath);
        menu.SetTexture(txtrPath);
    }

    if (j.find("isMain") != j.end())
    {
        from_json(j["isMain"], menu.isMain);
    }

    for (auto itr = j.begin(); itr != j.end(); itr = std::next(itr))
    {
        if (itr.key().compare("Size")       == 0 || itr.key().compare("First")  == 0 ||
            itr.key().compare("Background") == 0 || itr.key().compare("isMain") == 0 )
            continue;
        
        auto nButton = menu.AddButton(itr.key(),
            startPnt, std::make_pair(false,false),
            commonDmns, std::make_pair(false,false),
            glm::vec4(0)
        );
        startPnt.y += commonDmns.y + 10;
        from_json(itr.value(), *nButton);
    }
}


inline void to_json(ordered_json& j, Menu& menu)
{
    bool first = true;
    for (auto [name,button] : menu.buttons)
    {
        if  (first)
        {
            first = false;
            to_json(j["First"],button->GetPosition());
            to_json(j["Size"],button->GetDimensions());
        }
        to_json(j[name],*button);
    }
}



#endif

