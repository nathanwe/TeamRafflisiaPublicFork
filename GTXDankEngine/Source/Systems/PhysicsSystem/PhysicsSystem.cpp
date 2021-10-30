#include "pch.h"


#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"

#include "../utils/Log.h"
#include "PhysicsSystem.h"

//#include "../Components/PhysicsComponent/RigidBodyComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"


#include "../Core/FramerateControlSystem/FramerateController.h"
#include "../Core/Engine.h"

//#include "Collision/CollisionFunctions.h"

extern Engine engine;



bool PhysicsSystem::Init()
{
	return true;
}

void PhysicsSystem::Update(float timeStamp = 0)
{
	/*if (RigidBodyComponentPool.componentList.size() == 0)
		return;*/

	float dt = engine.Framerate->DeltaSeconds();

	//LOG_INFO("Delta time in Seconds {}", dt);



	for (const auto& [physicsEntity, movingBodyComponent] : MovingBodyComponentPool.componentList)
	{
		/*if (rigidBodyComponent->bodyType == Body_Type::STATIC)
			continue;*/
		Integrate(movingBodyComponent, dt);
	}

	//Detect Collision
	//DetectCollision();

	for (const auto& [physicsEntity, rigidBodyComponent] : MovingBodyComponentPool.componentList)
	{
		/*if (rigidBodyComponent->collider.shape == Shape::PLANE)
			continue;*/

		TransformComponentPool.componentList[physicsEntity]->transform->position = rigidBodyComponent->rigidBody->position;
		glm::quat qYaw  = glm::angleAxis(0.01f, glm::vec3(0, 1, 0));
		TransformComponentPool.componentList[physicsEntity]->transform->rotation *= glm::normalize(qYaw);
	}
}


//void PhysicsSystem::DetectCollision()
//{
//	//Hard Coded for now
//	for (auto itr1 = RigidBodyComponentPool.componentList.begin(); itr1 != RigidBodyComponentPool.componentList.end(); ++itr1)
//	{
//		for (auto itr2 = next(itr1, 1); itr2 != RigidBodyComponentPool.componentList.end(); ++itr2)
//		{
//			if (itr1->second->collider.shape == Shape::PLANE)
//			{
//				ReflectSpherePlane(itr1->second, itr2->second);
//			}
//			else if (itr2->second->collider.shape == Shape::PLANE)
//			{
//				ReflectSpherePlane(itr2->second, itr1->second);
//			}
//			else
//			{
//				ReflectSphereSphere(itr1->second, itr2->second);
//			}
//		}
//	}
//}



bool PhysicsSystem::Destroy()
{
	return true;
}


void PhysicsSystem::Integrate(MovingBodyComponent* movingBody, float dt)
{
	movingBody->rigidBody->prevVelocity = movingBody->rigidBody->velocity;
	movingBody->rigidBody->velocity += movingBody->rigidBody->acceleration * dt;

	if (movingBody->rigidBody->isGravity)
		if (movingBody->rigidBody->velocity.y < 0)
			movingBody->rigidBody->velocity += 0.5f * dt * glm::vec3(0, -1, 0);
		else
			movingBody->rigidBody->velocity += 0.3f * dt * glm::vec3(0, -1, 0);

	movingBody->rigidBody->prevPosition = movingBody->rigidBody->position;
	movingBody->rigidBody->position += movingBody->rigidBody->velocity * dt;

}