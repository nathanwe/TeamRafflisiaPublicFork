#pragma once

#include "Routine.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Systems/InputSystem/InputManager.h"

class LeftRight : public Routine
{
public:
	LeftRight(uint32_t entityID) : Routine{ entityID } {}

	void Update(float deltaTime, InputManager* InputSys)
	{
		TransformComponentPool.GetComponentByEntity(entityID)->transform.position.x += 1.0f * direction * deltaTime;
		timer += deltaTime;

		if (timer >= swapTime)
		{
			timer -= swapTime;
			direction *= -1;
		}

		if (InputSys->IsKeyPressed(GLFW_KEY_G))
		{
			TransformComponentPool.GetComponentByEntity(entityID)->transform.position.y += 1.0f * deltaTime;
		}

		if (InputSys->IsKeyPressed(GLFW_KEY_H))
		{
			TransformComponentPool.GetComponentByEntity(entityID)->transform.position.y -= 1.0f * deltaTime;
		}
	}

private:
	float timer = 0.f;
	float swapTime = 2.0f;
	float direction = 1.0f;


};
