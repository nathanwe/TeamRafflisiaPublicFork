#include "pch.h"
#include "Shadow.h"
#include "../utils/Log.h"
#include "../Core/Shader.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../utils/VQS.h"
#include "../Core/Model.h"

bool Shadow::Init(unsigned int shadowMapWidth, unsigned int shadowMapHeight)
{
    DefaultShader = new Shader("Source/Shaders/Shadow/DefaultShadowMap.shader");

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glGenTextures(1, &DepthBuffer);
    glBindTexture(GL_TEXTURE_2D, DepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // Clamp to border to fix over-sampling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthBuffer, 0);

    glGenTextures(1, &ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, shadowMapWidth, shadowMapHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // Clamp to border to fix over-sampling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer, 0);


    // check if framebuffer created successfullly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Shadow FBO failed to init.");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}




void Shadow::Destroy()
{
	delete DefaultShader;
}

void Shadow::Update()
{
    glm::vec3 target, lightPos;
    std::set<Entity> LTEntitys = LightComponentPool.Get2SharedEntitys(TransformComponentPool.componentList);
    for (Entity e : LTEntitys)
    {
        LightComponent* lightComponent = LightComponentPool.GetComponentByEntity(e);
        TransformComponent* transformComponent = TransformComponentPool.GetComponentByEntity(e);
        if (lightComponent->LightSource->Type == LightType::Directional)
        {
            target = lightComponent->LightSource->Target;
            lightPos = transformComponent->transform->position;
        }
    }

    //glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 1.0f, 50.0f);
    glm::mat4 lightProjection = glm::ortho(-Range, Range, -Range, Range, NearPlane, FarPlane);
    glm::mat4 lightView = glm::lookAt(lightPos, target, glm::vec3(0.0, 1.0, 0.0));

    LightSpaceMatrix = lightProjection * lightView;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DefaultShader->setMat4("view", lightView);
    DefaultShader->setMat4("projection", lightProjection);

    // render the scene
    Render(DefaultShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Shadow::Render(Shader* shader) const
{

    std::set<Entity> entitys = MaterialComponentPool.Get3SharedEntitys(TransformComponentPool.componentList, ModelComponentPool.componentList);
    for (auto e : entitys)
    {
        auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
        auto modelComponent = ModelComponentPool.GetComponentByEntity(e);

        shader->setMat4("model", transformComponent->transform->Matrix());
        modelComponent->model->GetPointer()->Draw(*shader);
    }
}


