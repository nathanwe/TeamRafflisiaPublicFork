#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "pch.h"

#include "../../Core/Camera.h"
#include "../../Core/System.h"
#include "Skybox/Skybox.h"
#include "Shadow/Shadow.h"
#include "DeferredRenderer/DeferredRenderer.h"
#include "FBO.h"
#include "PostProcess/PostProcess.h"
#include "DebugRender/DebugRender.h"
#include "OIT/OIT.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Sky/Sky.h"
#include "MenuSystem/MenuSystem.h"
#include "MenuSystem/Menu.h"
#include "TextRenderer/TextRenderer.h"



class Light;
class VQS;


const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 860;



class GraphicsSystem : public System
{
	
public:
	// default constructor
	GraphicsSystem() = default;
	
	// delete copy and assignment constructor
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator= (const GraphicsSystem&) = delete;

	void RenderUI(void);

	inline void SetPostProcessType(PostProcessType type) { postProcessType = type; }

public:
	bool Init();

	void Update(float timeStamp);

	bool Destroy();

	GLFWwindow* pWindow;

	Camera camera{ WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 10.0f) };



	// setter and getters
	inline void SetSunAngle(float angle) { m_sun_angle = angle; }
	inline void SetCelFactor(float factor) { DeferredRender.CelFraction = factor; }


	MenuSystem& GetMenuSystem() { return MenuSystem; };
	ParticleSystem& GetParticleSystem() { return PS; };

	void DrawCustomText(const std::string text, float scale, glm::vec2 pos, glm::vec3 color);

	bool drawLogo = false;
	void DrawLogo();

private:
	void InitGLFW();
	void InitWindow();
		
	void Render(float timeStamp);

	void BindLightSource(Shader* shader);
	void BindPointLight(Shader* shader, Light* light, VQS *transform, unsigned int index);
	void BindDirectionalLight(Shader* shader, Light* light, VQS* transform);


	void DebugDraw();

	void RendererFboResize(unsigned int width, unsigned int height);

	void SetSunDir(glm::vec3 dir);

	void DrawText(const std::string text, float scale, glm::vec2 pos, glm::vec3 color);

private:
	Skybox skybox;

	Shadow Shadow;

	ParticleSystem PS;

	DeferredRenderer DeferredRender;

	// post process and forward rendering FBO
	FBO HdrFBO;

	//OIT TransparentRenderer;

	PostProcess PostProcesser;

	DebugRender DebugRenderer;

	HosekWilkieSkyModel Sky;
	float m_sun_angle = 0.0;
	bool RenderingDebugMode = false;

	PostProcessType postProcessType;

	MenuSystem MenuSystem;

	TextRenderer TextRenderer;
	Shader* textShader;

	Shader* logoShader;
	unsigned int logoVAO;
	std::vector<float> logoVerts {-400,-100,0,0,
			    				   400, 100,1,1,
								  -400, 100,0,1,
								  -400,-100,0,0,
								   400,-100,1,0,
								   400, 100,1,1 };
	ResourceHandle<Texture>* ptrTxtr;
};
#endif // !GRAPHICSSYSTEM_H
