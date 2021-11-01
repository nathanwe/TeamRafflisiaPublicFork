#include "pch.h"


#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"

#include "../utils/Log.h"
#include "PhysicsSystem.h"

//#include "../Components/PhysicsComponent/RigidBodyComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"


#include "../Core/FramerateControlSystem/FramerateController.h"
#include "../Core/Engine.h"

#include "../ProfileSystem/ProfileSystem.h"
//#include "Collision/CollisionFunctions.h"

extern Engine engine;



bool PhysicsSystem::Init()
{
	return true;
}

void PhysicsSystem::Update(float timeStamp = 0)
{
	Timer timer("Physics Update");

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

	/*for (const auto& [physicsEntity, rigidBodyComponent] : RigidBodyComponentPool.componentList)
	{
		if (rigidBodyComponent->collider.shape == Shape::PLANE)
			continue;

		TransformComponentPool.componentList[physicsEntity]->transform->position = rigidBodyComponent->position;
	}*/
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
	movingBody->rigidBody.prevVelocity = movingBody->rigidBody.velocity;
	movingBody->rigidBody.velocity += movingBody->rigidBody.acceleration * dt;

	if (movingBody->rigidBody.isGravity)
		if (movingBody->rigidBody.velocity.y < 0)
			movingBody->rigidBody.velocity += 2.0f * dt * glm::vec3(0, -1, 0);
		else
			movingBody->rigidBody.velocity += 1.8f * dt * glm::vec3(0, -1, 0);

	movingBody->rigidBody.prevPosition = movingBody->rigidBody.position;
	movingBody->rigidBody.position += movingBody->rigidBody.velocity * dt;

}