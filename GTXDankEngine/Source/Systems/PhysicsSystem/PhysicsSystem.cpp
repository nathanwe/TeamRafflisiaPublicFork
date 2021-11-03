#include "pch.h"

////////
//
//
// Sphere radius at scale of 0.01 is 0.64 units
//
//
//
////

#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"

#include "../utils/Log.h"
#include "PhysicsSystem.h"

//#include "../Components/PhysicsComponent/RigidBodyComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"


#include "../Core/FramerateControlSystem/FramerateController.h"
#include "../Core/Engine.h"

#include "../ProfileSystem/ProfileSystem.h"
#include "Collision/CollisionFunctions.h"

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
	DetectCollision(dt);

	std::set<Entity> MTEntitys = MovingBodyComponentPool.Get2SharedEntitys(TransformComponentPool.componentList);
	for (auto e : MTEntitys)
	{
		/*if (rigidBodyComponent->collider.shape == Shape::PLANE)
			continue;*/
		auto movingBodyComponent = MovingBodyComponentPool.GetComponentByEntity(e);
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
		transformComponent->transform.position = movingBodyComponent->rigidBody.position;
		transformComponent->transform.rotation = movingBodyComponent->rigidBody.orientation;
		//TransformComponentPool.componentList[physicsEntity]->transform.position = movingBodyComponent->rigidBody.position;
		//TransformComponentPool.componentList[physicsEntity]->transform.rotation = movingBodyComponent->rigidBody.orientation;
		/*glm::quat qYaw  = glm::angleAxis(0.01f, glm::vec3(0, 1, 0));
		TransformComponentPool.componentList[physicsEntity]->transform->rotation *= glm::normalize(qYaw);*/
	}
}


void PhysicsSystem::DetectCollision(float dt)
{
	//Hard Coded for now
	// Moving Sphere - Moving Sphere
	for (auto itr1 = MovingBodyComponentPool.componentList.begin(); itr1 != MovingBodyComponentPool.componentList.end(); ++itr1)
	{
		for (auto itr2 = next(itr1, 1); itr2 != MovingBodyComponentPool.componentList.end(); ++itr2)
		{
			if (ReflectMovingSphereMovingSphere(itr1->second, itr2->second, dt))
			{
				Event ev = Event(true);
				ev.type = EventType::PHYSICS_COLLISION;
				ev.e1 = itr1->first;
				ev.e2 = itr2->first;
				engine.DoGameLogicScriptSys.HandleEvent(ev);
			}
		}
	}

	// Moving Sphere - Still Sphere
	for (auto itr1 = MovingBodyComponentPool.componentList.begin(); itr1 != MovingBodyComponentPool.componentList.end(); ++itr1)
	{
		for (auto itr2 = StillBodyComponentPool.componentList.begin() ; itr2 != StillBodyComponentPool.componentList.end(); ++itr2)
		{
			if(ReflectStaticSphereStaticSphere(itr1->second, itr2->second))
			{
				Event ev = Event(true);
				ev.type = EventType::PHYSICS_COLLISION;
				ev.e1 = itr1->first;
				ev.e2 = itr2->first;
				engine.DoGameLogicScriptSys.HandleEvent(ev);
			}
		}
	}
}

void PhysicsSystem::UpdatePosition()
{
	for (const auto& [physicsEntity, movingBodyComponent] : MovingBodyComponentPool.componentList)
	{
		movingBodyComponent->rigidBody.position = TransformComponentPool.componentList[physicsEntity]->transform.position;
		movingBodyComponent->rigidBody.orientation = TransformComponentPool.componentList[physicsEntity]->transform.rotation;

		

		movingBodyComponent->rigidBody.angularVelocity = glm::quat(1.0, 0.0, 0.0, 0.0);
		movingBodyComponent->rigidBody.torque = glm::quat(1.0, 0.0, 0.0, 0.0);
	}

	for (const auto& [physicsEntity, stillBodyComponent] : StillBodyComponentPool.componentList)
	{
		stillBodyComponent->position = TransformComponentPool.componentList[physicsEntity]->transform.position;
	}
}


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
			movingBody->rigidBody.velocity += movingBody->rigidBody.mass * dt * glm::vec3(0, -1, 0);
		else
			movingBody->rigidBody.velocity += 0.8f * movingBody->rigidBody.mass * dt * glm::vec3(0, -1, 0);


	movingBody->rigidBody.prevPosition = movingBody->rigidBody.position;
	movingBody->rigidBody.position += movingBody->rigidBody.velocity * dt;

	movingBody->rigidBody.prevAcceleration += movingBody->rigidBody.acceleration;


	//Rotation
	movingBody->rigidBody.prevAngularVelocity = movingBody->rigidBody.angularVelocity;
	//movingBody->rigidBody->angularVelocity *= movingBody->rigidBody->torque;

	movingBody->rigidBody.prevOrientation = movingBody->rigidBody.orientation;
	movingBody->rigidBody.orientation *= glm::normalize(movingBody->rigidBody.angularVelocity);

}