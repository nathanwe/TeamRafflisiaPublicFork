#ifndef _MENU_SLIDER_H_
#define _MENU_SLIDER_H_

#include "pch.h"
#include "../utils/SerializationHelper.h"

class Shader;
class Quad;
class Texture;

class MenuSlider
{
public:
    //MenuSlider();
    MenuSlider(std::string name, glm::vec2 nPos, glm::vec2 nDimensions);
    ~MenuSlider() {};

    MenuSlider* SetPosition(glm::vec2 nPos);
    MenuSlider* SetDimensions(glm::vec2 nDimensions);
    //MenuSlider* SetColor(glm::vec4 nRGBTint);

    void Draw(/*Shader& shader*/);
    void DrawPointer();

    void SetVertices();
    void SetPointerVertices();
    void SetPointerPosition();

    void Setup();


    const glm::vec2& GetPosition() const;
    const glm::vec2& GetDimensions() const;

    
    void AdjustForWindowSize();

    void SetDivision(int numOfParts);


public:
private:
private:
    std::string name;

    /// whether or not the percentages are static (val1-val2-val3 or continuos)
    bool continuous = true;
    int parts = 0;
    float part = 0;

    /// position of the slider (size universal based on height) / value of the slider
    float percent;
    bool pointerHeld = false;

    /// length and height of the actual slider
    /// generally universal between different sliders
    glm::vec2 position;                 /// middle point of quad
    glm::vec2 dimensions;

    /// real calculated position and dimensions
    float xSliderPos, xPointerPos, yCommonPos;
    float sldrWidth, sldrHeight;

    /// calculated vertices
    std::vector<float> vertices;
    std::vector<float> sliderVertices;

    
	unsigned int sliderVAO, pointerVAO;


    Shader* sliderShader;

    std::function<void()> actionToExecute;


    friend void button_to_json(ordered_json& j, MenuSlider& menuSldr);
    friend inline void to_json(ordered_json& j, MenuSlider& menuSldr);
};


void slider_from_json(const ordered_json& j, MenuSlider& menuSldr);
void slider_to_json(ordered_json& j, MenuSlider& menuSldr);


inline void from_json(const ordered_json& j, MenuSlider& menuSldr)
{
    slider_from_json(j, menuSldr);
}


inline void to_json(ordered_json& j, MenuSlider& menuSldr)
{
    slider_to_json(j, menuSldr);
}




#endif

