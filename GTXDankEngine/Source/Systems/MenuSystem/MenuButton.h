/*
    Written by Fedor Sharygin 2022
*/

#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include <pch.h>

class Shader;

/// Button class is a quad that can be hovered over
/// and pressed on with a LMB/A button
class MenuButton
{
    
/// methods
public:
    MenuButton();
    MenuButton(glm::vec2 nPos, glm::vec2 nDimensions, glm::vec3 nRGBTint,
        std::pair<bool, bool> nRelativePos = std::make_pair(false, false),
        std::pair<bool, bool> nRelativeDimns = std::make_pair(false, false));
    ~MenuButton() {};

    void SetPosition(glm::vec2 nPos, std::pair<bool, bool> nRelativePos = std::make_pair(false, false));
    void SetDimensions(glm::vec2 nDimensions, std::pair<bool, bool> nRelativeDimns = std::make_pair(false, false));
    void SetColor(glm::vec3 nRGBTint);

    void Draw(Shader& shader);

    void SetVertices();

    void Setup();

/// parameters
public:

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
    glm::vec3 rgbTint;

    /// calculated vertices
    std::vector<int> quadVerticesColorTexture;

    
	unsigned int VAO;
    std::vector<unsigned int> indices;
}


#endif

