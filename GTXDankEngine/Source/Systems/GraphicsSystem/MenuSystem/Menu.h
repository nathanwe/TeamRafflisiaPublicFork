/*
    Written by Fedor Sharygin 2022
*/

#ifndef _UI_MENU_
#define _UI_MENU_

#include "pch.h"


class MenuButton;
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

/// parameters
public:
    
/// methods
private:

/// parameters
private:
    std::map<std::string, MenuButton*> buttons;
    
    /// values for drawing the menu background
    std::vector<int> vertices;
	unsigned int VAO;
    std::vector<unsigned int> indices;
};



#endif

