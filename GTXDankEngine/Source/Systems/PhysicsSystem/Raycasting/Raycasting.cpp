
#include "pch.h"
#include "Raycasting.h"
#include "../Systems/PhysicsSystem/Collision/CollisionFunctions.h"
#include "../Components/PhysicsComponent/ColliderComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Core/Engine.h"

extern Engine engine;

glm::vec3 GetDirectionFromCameraCursor(Camera& camera)
{
	double screenX = camera.width;
	double screenY = camera.height;
	float mouseX = engine.InputSys.mouseX / (screenX * 0.5f) - 1.0f;
	float mouseY = engine.InputSys.mouseY / (screenY * 0.5f) - 1.0f;
	//LOG_INFO(mouseX);
	//LOG_INFO(mouseY);
	glm::mat4 projectionMatrix = camera.projectionMatrix;
	glm::mat4 viewMatrix = camera.viewMatrix;
	glm::mat4 inverseViewProjection = glm::inverse(projectionMatrix * viewMatrix);
	glm::vec4 screenPosition = glm::vec4(mouseX, -mouseY * screenY / screenX, 1.0f, 1.0f);
	glm::vec4 worldPosition = inverseViewProjection * screenPosition;
	return glm::vec3(worldPosition);
}

int RayCast(Camera& camera, bool isCenter)
{
	auto position = camera.GetPosition();
	auto rotation = glm::normalize(camera.orientationScale);

	int closestEntity = -1;
	float closestDistance = 100.0f;
	float distance = 100.0f;
	auto p1 = camera.GetPosition();
	auto velocity = isCenter ? camera.orientationScale : GetDirectionFromCameraCursor(camera);
	//auto velocity = camera.orientationScale;
	velocity *= 100.0f;
	auto p2 = p1 + velocity;
	for (const auto& [entity, collider] : ColliderComponentPool.componentList)
	{
		auto* trans = TransformComponentPool.GetComponentByEntity(entity);
		if (collider->NarrowPhase.shape == Shape::SPHERE)
		{
			distance = DynamicPointToStaticSphere(&p1, &p2, &velocity, &trans->transform.position, collider->NarrowPhase.radius);
		}
		else if (collider->NarrowPhase.shape == Shape::PLANE)
		{
			distance = DynamicPointToStaticPlane(&p1, &p2, &velocity, &collider->NarrowPhase.normal, collider->NarrowPhase.magnitude);
		}
		else if (collider->NarrowPhase.shape == Shape::AABB)
		{
			distance = DynamicPointToStaticAABB(&p1, &p2, &velocity, &trans->transform.position, &collider->NarrowPhase.maxPoint, &collider->NarrowPhase.minPoint, 1.0f);
		}
		else
		{
			LOG_ERROR("Raycasting Collision detection function not implemented");
			distance = 200.0f;
		}
		if (distance > 0 && distance < closestDistance)
		{
			closestDistance = distance;
			closestEntity = entity;
		}

	}
	return closestEntity;
}
