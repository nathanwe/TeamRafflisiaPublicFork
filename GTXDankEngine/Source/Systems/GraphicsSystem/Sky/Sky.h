

#pragma once
#include "pch.h"

class Shader;

// An Analytic Model for Full Spectral Sky-Dome Radiance (Lukas Hosek, Alexander Wilkie)
class HosekWilkieSkyModel 
{
public:
	HosekWilkieSkyModel();
	~HosekWilkieSkyModel();

	inline float turbidity() { return m_turbidity; }
	inline void set_turbidity(float t) { m_turbidity = t; }

	bool initialize();
	void update();
	void set_render_uniforms(Shader* shader, glm::mat4 view, glm::mat4 projection);
	void Render(glm::mat4 view, glm::mat4 projection, unsigned int FBO);

	void Destroy() { delete shader; }

	glm::vec3 GetSunDir();

private:
	glm::vec3 A, B, C, D, E, F, G, H, I;
	glm::vec3 Z;
	float m_normalized_sun_y = 1.15f;
	float m_albedo = 0.1f;
	float m_turbidity = 4.0f;

	Shader* shader;
};