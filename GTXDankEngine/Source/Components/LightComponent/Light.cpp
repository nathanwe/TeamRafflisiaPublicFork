#include "pch.h"
#include "Light.h"
#include "../utils/Log.h"

Light::Light(LightType type, glm::vec3 c, glm::vec3 i)
	: Type(type), Color(c), Intensity(i)
{
	Color.x = Clip(Color.x, 1.0, 0.0);
	Color.y = Clip(Color.y, 1.0, 0.0);
	Color.z = Clip(Color.z, 1.0, 0.0);

	if (Intensity.x < 0) Intensity.x = 0;
	if (Intensity.y < 0) Intensity.y = 0;
	if (Intensity.z < 0) Intensity.z = 0;

	
#if defined(_DEBUG ) || defined(DEBUG)
	LOG_INFO("Creating Light with color: {0}, {1}, {2}", Color.x, Color.y, Color.z);
	LOG_INFO("Creating Light with Intensity: {0}, {1}, {2}", Intensity.x, Intensity.y, Intensity.z);
	LOG_INFO("Creating Light with Type: {0}", 
		type == LightType::Directional ? "Directional" : "Point");
#endif
}


float Light::Clip(float target, float max, float min)
{
	if (target < min) return min;
	if (target > max) return max;
	return target;
}