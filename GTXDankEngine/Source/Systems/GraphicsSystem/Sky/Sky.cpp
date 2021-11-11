#include "pch.h"
#include "Sky.h"
#include "Coefficients.h"
#include "../Core/Shader.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/LightComponent/LightComponent.h"

#define M_PI 3.1415

double evaluate_spline(const double* spline, size_t stride, double value)
{
    return
        1 * pow(1 - value, 5) * spline[0 * stride] +
        5 * pow(1 - value, 4) * pow(value, 1) * spline[1 * stride] +
        10 * pow(1 - value, 3) * pow(value, 2) * spline[2 * stride] +
        10 * pow(1 - value, 2) * pow(value, 3) * spline[3 * stride] +
        5 * pow(1 - value, 1) * pow(value, 4) * spline[4 * stride] +
        1 * pow(value, 5) * spline[5 * stride];
}



double evaluate(const double* dataset, size_t stride, float turbidity, float albedo, float sunTheta)
{
    // splines are functions of elevation^1/3
    double elevationK = pow(std::max<float>(0.f, 1.f - sunTheta / (M_PI / 2.f)), 1.f / 3.0f);

    // table has values for turbidity 1..10
    int turbidity0 = glm::clamp(static_cast<int>(turbidity), 1, 10);
    int turbidity1 = std::min(turbidity0 + 1, 10);
    float turbidityK = glm::clamp(turbidity - turbidity0, 0.f, 1.f);

    const double* datasetA0 = dataset;
    const double* datasetA1 = dataset + stride * 6 * 10;

    double a0t0 = evaluate_spline(datasetA0 + stride * 6 * (turbidity0 - 1), stride, elevationK);
    double a1t0 = evaluate_spline(datasetA1 + stride * 6 * (turbidity0 - 1), stride, elevationK);
    double a0t1 = evaluate_spline(datasetA0 + stride * 6 * (turbidity1 - 1), stride, elevationK);
    double a1t1 = evaluate_spline(datasetA1 + stride * 6 * (turbidity1 - 1), stride, elevationK);

    return a0t0 * (1 - albedo) * (1 - turbidityK) + a1t0 * albedo * (1 - turbidityK) + a0t1 * (1 - albedo) * turbidityK + a1t1 * albedo * turbidityK;
}




glm::vec3 hosek_wilkie(float cos_theta, float gamma, float cos_gamma, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, glm::vec3 E, glm::vec3 F, glm::vec3 G, glm::vec3 H, glm::vec3 I)
{
    glm::vec3 chi = (1.f + cos_gamma * cos_gamma) / pow(1.f + H * H - 2.f * cos_gamma * H, glm::vec3(1.5f));
    return (1.f + A * exp(B / (cos_theta + 0.01f))) * (C + D * exp(E * gamma) + F * (cos_gamma * cos_gamma) + G * chi + I * (float)sqrt(std::max(0.f, cos_theta)));
}



HosekWilkieSkyModel::HosekWilkieSkyModel()
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

HosekWilkieSkyModel::~HosekWilkieSkyModel()
{

}


bool HosekWilkieSkyModel::initialize()
{
    shader = new Shader("Source/Shaders/Sky/Sky.shader");
    return true;
}



void HosekWilkieSkyModel::update()
{
    glm::vec3 sunDir = GetSunDir();
    const float sunTheta = std::acos(glm::clamp(sunDir.y, 0.f, 1.f));

    for (int i = 0; i < 3; ++i)
    {
        A[i] = evaluate(datasetsRGB[i] + 0, 9, m_turbidity, m_albedo, sunTheta);
        B[i] = evaluate(datasetsRGB[i] + 1, 9, m_turbidity, m_albedo, sunTheta);
        C[i] = evaluate(datasetsRGB[i] + 2, 9, m_turbidity, m_albedo, sunTheta);
        D[i] = evaluate(datasetsRGB[i] + 3, 9, m_turbidity, m_albedo, sunTheta);
        E[i] = evaluate(datasetsRGB[i] + 4, 9, m_turbidity, m_albedo, sunTheta);
        F[i] = evaluate(datasetsRGB[i] + 5, 9, m_turbidity, m_albedo, sunTheta);
        G[i] = evaluate(datasetsRGB[i] + 6, 9, m_turbidity, m_albedo, sunTheta);

        // Swapped in the dataset
        H[i] = evaluate(datasetsRGB[i] + 8, 9, m_turbidity, m_albedo, sunTheta);
        I[i] = evaluate(datasetsRGB[i] + 7, 9, m_turbidity, m_albedo, sunTheta);

        Z[i] = evaluate(datasetsRGBRad[i], 1, m_turbidity, m_albedo, sunTheta);
    }

    if (m_normalized_sun_y)
    {
        glm::vec3 S = hosek_wilkie(std::cos(sunTheta), 0, 1.f, A, B, C, D, E, F, G, H, I) * Z;
        Z /= glm::dot(S, glm::vec3(0.2126, 0.7152, 0.0722));
        Z *= m_normalized_sun_y;
    }
}


void HosekWilkieSkyModel::set_render_uniforms(Shader* shader, glm::mat4 view, glm::mat4 projection)
{
    shader->setVec3("u_Direction", GetSunDir());
    shader->setVec3("A", A);
    shader->setVec3("B", B);
    shader->setVec3("C", C);
    shader->setVec3("D", D);
    shader->setVec3("E", E);
    shader->setVec3("F", F);
    shader->setVec3("G", G);
    shader->setVec3("H", H);
    shader->setVec3("I", I);
    shader->setVec3("Z", Z);

    shader->setMat4("View", view);
    shader->setMat4("Proj", projection);
}


void HosekWilkieSkyModel::Render(glm::mat4 view, glm::mat4 projection, unsigned int FBO)
{
    update();
    set_render_uniforms(shader, view, projection);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    shader->Bind();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    shader->unBind();

    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



glm::vec3 HosekWilkieSkyModel::GetSunDir()
{
    glm::vec3 target, lightPos;
    std::set<Entity> LTEntitys = LightComponentPool.Get2SharedEntitys(TransformComponentPool.componentList);
    for (Entity e : LTEntitys)
    {
        LightComponent* lightComponent = LightComponentPool.GetComponentByEntity(e);
        TransformComponent* transformComponent = TransformComponentPool.GetComponentByEntity(e);
        if (lightComponent->LightSource.Type == LightType::Directional)
        {
            target = lightComponent->LightSource.Target;
            lightPos = transformComponent->transform.position;
        }
    }

    return  glm::normalize( lightPos- target);
}