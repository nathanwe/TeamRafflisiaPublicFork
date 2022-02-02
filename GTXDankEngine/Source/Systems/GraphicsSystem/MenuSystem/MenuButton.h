/*
    Written by Fedor Sharygin 2022
*/

#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include "pch.h"
#include "../utils/SerializationHelper.h"

class Shader;
class Quad;
class Texture;

/// Button class is a quad that can be hovered over
/// and pressed on with a LMB/A button
class MenuButton
{
    
/// methods
public:
    MenuButton();
    MenuButton(glm::vec2 nPos, glm::vec2 nDimensions, glm::vec4 nRGBTint,
        std::pair<bool, bool> nRelativePos = std::make_pair(false, false),
        std::pair<bool, bool> nRelativeDimns = std::make_pair(false, false));
    ~MenuButton() {};

    MenuButton* SetPosition(glm::vec2 nPos, std::pair<bool, bool> nRelativePos = std::make_pair(false, false));
    MenuButton* SetDimensions(glm::vec2 nDimensions, std::pair<bool, bool> nRelativeDimns = std::make_pair(false, false));
    MenuButton* SetColor(glm::vec4 nRGBTint);

    void Draw(Shader& shader);

    void SetVertices();

    void Setup();

    
    MenuButton* SetActionToExecute(std::function<void()> actionToExecute);
    void Execute();


    MenuButton* SetTexture(std::string texturePath);


    const glm::vec2& GetPosition() const;
    const glm::vec2& GetDimensions() const;

/// parameters
public:
    std::string nMenu;

/// methods
private:

/// parameters
private:
    /// if we want to use relative values for position
    std::pair<bool, bool> relativePos;
    glm::vec2 position;                 /// middle point of quad
    
    /// if we want to use relative values for dimensions
    std::pair<bool, bool> relativeDimns;
    glm::vec2 dimensions;

    /// color tint
    glm::vec4 rgbTint;

    /// calculated vertices
    std::vector<float> quadVerticesColorTexture;

    
	unsigned int VAO;
    std::vector<unsigned int> indices;

    float xPos, yPos;
    float btnWidth, btnHeight;


    bool readyToExecute = false;
    std::function<void()> actionToExecute;

    bool haveTexture = false;
    std::string buttonTexture;
    
    friend void button_to_json(ordered_json& j, MenuButton& menuBut);
    friend inline void to_json(ordered_json& j, MenuButton& menuBut);
};


void button_from_json(const ordered_json& j, MenuButton& menuBut);
void button_to_json(ordered_json& j, MenuButton& menuBut);


inline void from_json(const ordered_json& j, MenuButton& menuBut)
{
    button_from_json(j, menuBut);
}


inline void to_json(ordered_json& j, MenuButton& menuBut)
{
    button_to_json(j, menuBut);
}



#endif

