#include "pch.h"

#include "MenuSlider.h"

#include "../Core/Shader.h"
#include "../Core/Texture.h"

/// used to get WIDTH and HEIGHT
#include "../Systems/GraphicsSystem/GraphicsSystem.h"


#include "../Core/Engine.h"


/// Engine is used to access the window, camera and the GraphicsSystem
extern Engine engine;



MenuSlider::MenuSlider(std::string name, glm::vec2 nPos, glm::vec2 nDimensions)
    :
name(name), position(nPos), dimensions(nDimensions), percent(0)
{
    SetVertices();
}


MenuSlider* MenuSlider::SetPosition(glm::vec2 nPos)
{
    position = nPos;
    SetVertices();
    return this;
}

MenuSlider* MenuSlider::SetDimensions(glm::vec2 nDimensions)
{
    dimensions = nDimensions;
    SetVertices();
    return this;
}


void MenuSlider::Draw()
{
    /// Draw the name of the slider above the slider
    engine.GraphicsSys.DrawCustomText(name.c_str(), (sldrHeight+5) / 100.0f, glm::vec2(xSliderPos-sldrWidth/2.0f, yCommonPos+sldrHeight), glm::vec3(1));


    double xMPos, yMPos;
    glfwGetCursorPos(engine.window, &xMPos, &yMPos);


    /// Check if we are clicking on the pointer
    if (std::abs(xMPos - xPointerPos) <= sldrHeight / 2.0f && std::abs(yMPos - position.y) <= sldrHeight / 2.0f && engine.InputSys.IsLeftMousePressed() && !pointerHeld)
    {
        pointerHeld = true;


        /// Draw the current number to be used
        std::string percentile;
        if (continuous)
        {
            percentile = std::to_string(float(int(percent*10000))/100.0f);
            percentile = percentile.substr(0,percentile.size()-4)+"%";
        }
        else
        {
            float curPart = percent / part;
            if (percent / part - float(int(percent / part)) > 0.5f)
                percentile = std::to_string(int(percent / part)+2);
            else
                percentile = std::to_string(int(percent / part)+1);
        }
        engine.GraphicsSys.DrawCustomText(percentile, (sldrHeight+5) / 100.0f, glm::vec2(xPointerPos-sldrHeight/2.0f, yCommonPos-sldrHeight-3), glm::vec3(1));
    }

    /// As soon as the mouse is released => communicate the results
    if (pointerHeld && !engine.InputSys.IsLeftMousePressed())
    {
        pointerHeld = false;
        if (!continuous)
        {
            float curPart = percent / part;
            if (percent / part - float(int(percent / part)) > 0.5f)
                percent = (float(int(percent / part)) + 1) * part;
            else
                percent = float(int(percent / part)) * part;
            this->SetPointerPosition();
            if (name == "BGM Volume")
                engine.AudioSys.BGMVolume = int(percent / part)+1;
            else if (name == "SFX Volume")
                engine.AudioSys.SFXVolume = int(percent / part)+1;
        }
    }
    else
    /// While the mouse is held => move the slider
    if (pointerHeld && engine.InputSys.IsLeftMousePressed())
    {
        auto change = (xMPos - xPointerPos) / (sldrWidth - sldrHeight);
        percent += change;
        if (percent < 0)
            percent = 0;
        else if (percent > 1)
            percent = 1;


        /// Draw the current number to be used
        std::string percentile;
        if (continuous)
        {
            percentile = std::to_string(float(int(percent*10000))/100.0f);
            percentile = percentile.substr(0,percentile.size()-4)+"%";
        }
        else
        {
            float curPart = percent / part;
            if (percent / part - float(int(percent / part)) > 0.5f)
                percentile = std::to_string(int(percent / part)+2);
            else
                percentile = std::to_string(int(percent / part)+1);
        }
        engine.GraphicsSys.DrawCustomText(percentile, (sldrHeight+5) / 100.0f, glm::vec2(xPointerPos-sldrHeight/2.0f, yCommonPos-sldrHeight-3), glm::vec3(1));


        this->SetPointerPosition();
    }

    sliderShader->setMat4("projection", glm::ortho(0.0f,float(engine.GraphicsSys.camera.width),0.0f,float(engine.GraphicsSys.camera.height)));

    /// draw slider
    sliderShader->setVec2("centerLocation", glm::vec2(xSliderPos,yCommonPos));
    sliderShader->Bind();
    glBindVertexArray(sliderVAO);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    sliderShader->unBind();

    /// draw pointer
    this->DrawPointer();
}

void MenuSlider::DrawPointer()
{
    sliderShader->setVec2("centerLocation", glm::vec2(xPointerPos,yCommonPos));
    auto ptrTxtr = TextureResourceManger.GetResourceHandle("Assets/Textures/SliderTextureRed.png");
    sliderShader->setTexture("txtr", ptrTxtr->GetPointer()->GetID());
    sliderShader->Bind();
    glBindVertexArray(pointerVAO);
    glDisable(GL_DEPTH_TEST);
//    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    sliderShader->unBind();
}


void MenuSlider::SetVertices()
{
    sldrWidth = dimensions.x;
    sldrHeight = dimensions.y;
    this->SetYPosition();


    xSliderPos = position.x;
    /// setup slider vertices
    /// first triangle
    vertices.push_back(/*xSliderPos*/ - sldrWidth / 2.0f); vertices.push_back(/*yCommonPos*/ - sldrHeight / 2.0f); vertices.push_back(-1); vertices.push_back(-1);
    vertices.push_back(/*xSliderPos*/ + sldrWidth / 2.0f); vertices.push_back(/*yCommonPos*/ + sldrHeight / 2.0f); vertices.push_back(-1); vertices.push_back(-1);
    vertices.push_back(/*xSliderPos*/ - sldrWidth / 2.0f); vertices.push_back(/*yCommonPos*/ + sldrHeight / 2.0f); vertices.push_back(-1); vertices.push_back(-1);
    /// second triangle
    vertices.push_back(/*xSliderPos*/ - sldrWidth / 2.0f); vertices.push_back(/*yCommonPos*/ - sldrHeight / 2.0f); vertices.push_back(-1); vertices.push_back(-1);
    vertices.push_back(/*xSliderPos*/ + sldrWidth / 2.0f); vertices.push_back(/*yCommonPos*/ - sldrHeight / 2.0f); vertices.push_back(-1); vertices.push_back(-1);
    vertices.push_back(/*xSliderPos*/ + sldrWidth / 2.0f); vertices.push_back(/*yCommonPos*/ + sldrHeight / 2.0f); vertices.push_back(-1); vertices.push_back(-1);

    /// setup pointer vertices
    SetPointerVertices();
}

void MenuSlider::SetYPosition()
{
    yCommonPos = engine.GraphicsSys.camera.height - position.y;
}


void MenuSlider::SetPointerPosition()
{
    xPointerPos = position.x - (sldrWidth - sldrHeight) / 2.0f + (sldrWidth - sldrHeight) * percent;
}

void MenuSlider::SetPointerVertices()
{
    this->SetPointerPosition();

    /// first triangle
    sliderVertices.push_back(/*xPointerPos*/ - sldrHeight / 2.0f); sliderVertices.push_back(/*yCommonPos*/ - sldrHeight / 2.0f); sliderVertices.push_back(0); sliderVertices.push_back(0);
    sliderVertices.push_back(/*xPointerPos*/ + sldrHeight / 2.0f); sliderVertices.push_back(/*yCommonPos*/ + sldrHeight / 2.0f); sliderVertices.push_back(1); sliderVertices.push_back(1);
    sliderVertices.push_back(/*xPointerPos*/ - sldrHeight / 2.0f); sliderVertices.push_back(/*yCommonPos*/ + sldrHeight / 2.0f); sliderVertices.push_back(0); sliderVertices.push_back(1);
    /// second triangle
    sliderVertices.push_back(/*xPointerPos*/ - sldrHeight / 2.0f); sliderVertices.push_back(/*yCommonPos*/ - sldrHeight / 2.0f); sliderVertices.push_back(0); sliderVertices.push_back(0);
    sliderVertices.push_back(/*xPointerPos*/ + sldrHeight / 2.0f); sliderVertices.push_back(/*yCommonPos*/ - sldrHeight / 2.0f); sliderVertices.push_back(1); sliderVertices.push_back(0);
    sliderVertices.push_back(/*xPointerPos*/ + sldrHeight / 2.0f); sliderVertices.push_back(/*yCommonPos*/ + sldrHeight / 2.0f); sliderVertices.push_back(1); sliderVertices.push_back(1);
}


void MenuSlider::Setup()
{
    /// slider setup
    // VAO
    glGenVertexArrays(1, &sliderVAO);
    glBindVertexArray(sliderVAO);

    unsigned int sliderVBO;
    // VBO
    //VertexBuffer vbo(&quadVerticesColorTexture[0], quadVerticesColorTexture.size() * sizeof(Vertex));
    glGenBuffers(1, &sliderVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sliderVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // vertex texture coordinates (-1,-1) to signify grey color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    /// pointer setup
    // VAO
    glGenVertexArrays(1, &pointerVAO);
    glBindVertexArray(pointerVAO);

    unsigned int pointerVBO;
    // VBO
    //VertexBuffer vbo(&quadVerticesColorTexture[0], quadVerticesColorTexture.size() * sizeof(Vertex));
    glGenBuffers(1, &pointerVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointerVBO);
    glBufferData(GL_ARRAY_BUFFER, sliderVertices.size() * sizeof(float), sliderVertices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // vertex texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    sliderShader = new Shader("Source/Shaders/MenuRenderer/SliderPointerShader.shader");
}


const glm::vec2& MenuSlider::GetPosition() const
{
    return position;
}

const glm::vec2& MenuSlider::GetDimensions() const
{
    return dimensions;
}


void MenuSlider::AdjustForWindowSize()
{
    SetVertices();
    Setup();
}


void MenuSlider::SetDivision(int numOfParts)
{
    parts = numOfParts;
    continuous = false;
    part = 1.0f / float(parts-1);
    if (!name.empty())
    {
        if (name == "BGM Volume")
            percent = (engine.AudioSys.BGMVolume-1) * part;
        else if (name == "SFX Volume")
            percent = (engine.AudioSys.SFXVolume-1) * part;

        this->SetPointerPosition();
    }
}




void slider_from_json(const ordered_json& j, MenuSlider& menuSldr)
{
    if (j.find("Parts") != j.end())
    {
        int partNum;
        from_json(j["Parts"], partNum);
        menuSldr.SetDivision(partNum);
    }
}


void slider_to_json(ordered_json& j, MenuSlider& menuSldr)
{}



