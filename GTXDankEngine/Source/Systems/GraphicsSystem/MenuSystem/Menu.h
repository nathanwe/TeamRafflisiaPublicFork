/*
    Written by Fedor Sharygin 2022
*/

#ifndef _UI_MENU_
#define _UI_MENU_

#include "pch.h"
#include "../utils/SerializationHelper.h"

#include "MenuButton.h"
#include "MenuSlider.h"


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
    
    MenuSlider* AddSlider(std::string name,
        glm::vec2 position, glm::vec2 dimensions);
    MenuSlider* GetSlider(std::string name);

    void Draw(Shader& shader);
    void Setup();

    void SetTexture(std::string background);
    
    void AdjustForWindowSize();

/// parameters
public:
    
/// methods
private:

/// parameters
private:
    std::map<std::string, MenuButton*> buttons;
    std::map<std::string, MenuSlider*> sliders;
    
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
    glm::vec2 buttonDmns, sliderDmns, startPnt;
    bool isCenter = false;
    from_json(j["Button Size"], buttonDmns);
    from_json(j["Slider Size"], sliderDmns);

    if (j["First"]["x"].is_number())
        from_json(j["First"], startPnt);
    else if (j["First"]["x"].get<std::string>() == "center")
    {
        isCenter = true;
        from_json(j["First"]["y"], startPnt.y);
        startPnt.x = 0.5f;
    }

    if (j.find("Background") != j.end())
    {
        std::string txtrPath;
        from_json(j["Background"], txtrPath);
        menu.SetTexture(txtrPath);
    }

    for (auto itr = j.begin(); itr != j.end(); itr = std::next(itr))
    {
        if (itr.key().compare("Button Size") == 0 || itr.key().compare("First") == 0 || itr.key().compare("Slider Size") == 0 ||
            itr.key().compare("Background") == 0)
            continue;

        bool isSlider = false;
        if (itr.value().find("Slider") != itr.value().end())
            from_json(itr.value()["Slider"], isSlider);
        
        if (!isSlider)
        {
            auto nButton = menu.AddButton(itr.key(),
                startPnt, std::make_pair(isCenter, false),
                buttonDmns, std::make_pair(false, false),
                glm::vec4(0)
            );
            from_json(itr.value(), *nButton);
        }
        else
        {
            auto nSlider = menu.AddSlider(itr.key(),
                //startPnt - buttonDmns/2.0f + sliderDmns/2.0f,
                startPnt + glm::vec2((sliderDmns.x-buttonDmns.x)/2.0f, 0),
                sliderDmns);
            from_json(itr.value(), *nSlider);
        }
        startPnt.y += buttonDmns.y + 10;
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

