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
#include "../Components/PhysicsComponent/ColliderComponent.h"

#include "../utils/Log.h"
#include "PhysicsSystem.h"

//#include "../Components/PhysicsComponent/RigidBodyComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"


#include "../Core/FramerateControlSystem/FramerateController.h"
#include "../Core/Engine.h"

#include "../ProfileSystem/ProfileSystem.h"
#include "Collision/CollisionFunctions.h"

#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

extern Engine engine;


bool PhysicsSystem::Init()
{
	return true;
}

void PhysicsSystem::Update(float time = 0)
{
	PROFILE_THIS("Physics Update");

	/*if (RigidBodyComponentPool.componentList.size() == 0)
		return;*/

	float dt = engine.Framerate->DeltaSeconds();

	//LOG_INFO("Delta time in Seconds {}", dt);

	UpdateMovingColliders();

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
	// Moving - Moving
	for (auto itr1 = MovingBodyComponentPool.componentList.begin(); itr1 != MovingBodyComponentPool.componentList.end(); ++itr1)
	{
		for (auto itr2 = next(itr1, 1); itr2 != MovingBodyComponentPool.componentList.end(); ++itr2)
		{
			ColliderComponent* c1 = ColliderComponentPool.GetComponentByEntity(itr1->first);
			ColliderComponent* c2 = ColliderComponentPool.GetComponentByEntity(itr2->first);

			// SPHERE - SPHERE
			if (c1->NarrowPhase.shape == Shape::SPHERE && c2->NarrowPhase.shape == Shape::SPHERE)
			{
				if (ReflectMovingSphereMovingSphere(itr1->second, c1, itr2->second, c2, dt))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
			}
			// AABB - AABB
			if (c1->NarrowPhase.shape == Shape::AABB && c2->NarrowPhase.shape == Shape::AABB)
			{
				if (ReflectMovingAABBMovingAABB(itr1->second, c1, itr2->second, c2))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
			}
		}
	}

	// Moving - Still
	for (auto itr1 = MovingBodyComponentPool.componentList.begin(); itr1 != MovingBodyComponentPool.componentList.end(); ++itr1)
	{
		for (auto itr2 = StillBodyComponentPool.componentList.begin(); itr2 != StillBodyComponentPool.componentList.end(); ++itr2)
		{
			ColliderComponent* c1 = ColliderComponentPool.GetComponentByEntity(itr1->first);
			ColliderComponent* c2 = ColliderComponentPool.GetComponentByEntity(itr2->first);

			// Sphere - Sphere
			if (c1->NarrowPhase.shape == Shape::SPHERE && c2->NarrowPhase.shape == Shape::SPHERE)
			{
				if (ReflectMovingSphereStaticSphere(itr1->second,c1, itr2->second, c2, dt))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
			}
			// Sphere - Plane
			else if (c1->NarrowPhase.shape == Shape::SPHERE && c2->NarrowPhase.shape == Shape::PLANE)
			{

				if (ReflectMovingSphereStaticPlane(itr1->second, c1, itr2->second, c2, dt))
				{
					SendEvent(itr1->first, itr2->first, dt);
					
				}
			}
			// AABB - AABB
			else if (c1->NarrowPhase.shape == Shape::AABB && c2->NarrowPhase.shape == Shape::AABB)
			{
				if (ReflectMovingAABBStaticAABB(itr1->second, c1, itr2->second, c2))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
			}
			// AABB - Plane
			/*else if (c1->NarrowPhase.shape == Shape::AABB && c2->NarrowPhase.shape == Shape::PLANE)
			{
				if (ReflectMovingAABBStaticPlane(itr1->second, c1, itr2->second, c2))
				{
					Event ev = Event(true);
					ev.type = EventType::PHYSICS_COLLISION;
					ev.e1 = itr1->first;
					ev.e2 = itr2->first;
					engine.DoGameLogicScriptSys.HandleEvent(ev);
				}
			}*/
			// Sphere - AABB
			else if (c1->NarrowPhase.shape == Shape::SPHERE && c2->NarrowPhase.shape == Shape::AABB)
			{
				if (ReflectMovingSphereStaticAABB(itr1->second, c1, itr2->second, c2, dt))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
			}
			// AABB - AABB
			else if (c1->NarrowPhase.shape == Shape::AABB && c2->NarrowPhase.shape == Shape::AABB)
			{
				if (ReflectMovingAABBStaticAABB(itr1->second, c1, itr2->second, c2))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
			}
			// AABB - Plane
			/*else if (c1->NarrowPhase.shape == Shape::AABB && c2->NarrowPhase.shape == Shape::PLANE)
			{
				if (ReflectMovingAABBStaticPlane(itr1->second, c1, itr2->second, c2))
				{
					Event ev = Event(true);
					ev.type = EventType::PHYSICS_COLLISION;
					ev.e1 = itr1->first;
					ev.e2 = itr2->first;
					engine.DoGameLogicScriptSys.HandleEvent(ev);
				}
			}*/
			// Sphere - AABB
			else if (c1->NarrowPhase.shape == Shape::SPHERE && c2->NarrowPhase.shape == Shape::AABB)
			{
				if (ReflectMovingSphereStaticAABB(itr1->second, c1, itr2->second, c2, dt))
				{
					SendEvent(itr1->first, itr2->first, dt);
				}
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

		// Updating Posiyion of Collider Component
		//ColliderComponentPool.componentList[physicsEntity]->position = TransformComponentPool.componentList[physicsEntity]->transform.position;

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

void PhysicsSystem::SendEvent(Entity e1, Entity e2, float dt)
{
	auto GL1 = GameLogicCategoryComponentPool.GetComponentByEntity(e1);
	if (GL1 != nullptr)
	{
		Event eva = Event();
		eva.thingsToEffect = GL1->categories;
		eva.type = EventType::PHYSICS_COLLISION;
		eva.e1 = e1;
		eva.e2 = e2;
		eva.floatData1 = dt;
		engine.DoGameLogicScriptSys.HandleEvent(eva);
	}

	auto GL2 = GameLogicCategoryComponentPool.GetComponentByEntity(e2);
	if (GL2 != nullptr)
	{
		Event evb = Event();
		evb.thingsToEffect = GL2->categories;
		evb.type = EventType::PHYSICS_COLLISION;
		evb.e1 = e2;
		evb.e2 = e1;
		evb.floatData1 = dt;
		engine.DoGameLogicScriptSys.HandleEvent(evb);
	}
}



void PhysicsSystem::Integrate(MovingBodyComponent* movingBody, float dt)
{
	movingBody->rigidBody.prevVelocity = movingBody->rigidBody.velocity;
	movingBody->rigidBody.velocity += movingBody->rigidBody.acceleration * dt;


	if (movingBody->rigidBody.isGravity)
		if (movingBody->rigidBody.velocity.y < 0)
			movingBody->rigidBody.velocity += movingBody->rigidBody.mass * GRAVITY * dt * glm::vec3(0, -1, 0);
		else
			movingBody->rigidBody.velocity += 1.2f * movingBody->rigidBody.mass * GRAVITY * dt * glm::vec3(0, -1, 0);


	movingBody->rigidBody.prevPosition = movingBody->rigidBody.position;
	movingBody->rigidBody.position += movingBody->rigidBody.velocity * dt;

	movingBody->rigidBody.prevAcceleration += movingBody->rigidBody.acceleration;


	//Rotation
	movingBody->rigidBody.prevAngularVelocity = movingBody->rigidBody.angularVelocity;
	//movingBody->rigidBody->angularVelocity *= movingBody->rigidBody->torque;

	movingBody->rigidBody.prevOrientation = movingBody->rigidBody.orientation;
	movingBody->rigidBody.orientation *= glm::normalize(movingBody->rigidBody.angularVelocity);

}

void PhysicsSystem::UpdateSingleCollider(Entity e)
{
	auto colliderComponent = ColliderComponentPool.GetComponentByEntity(e);
	auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
	if (colliderComponent != nullptr && transformComponent != nullptr)
	{
		colliderComponent->setPostion(transformComponent->transform.position);

		if (colliderComponent->NarrowPhase.shape == Shape::SPHERE)
		{

		}
		else if (colliderComponent->NarrowPhase.shape == Shape::PLANE)
		{
			glm::vec3 up = glm::vec3(0, 1, 0);
			glm::vec3 right = glm::vec3(1, 0, 0);
			glm::vec3 out = glm::vec3(0, 0, 1);

			//Scale
			/*right *= transformComponent->transform.scale.x;
			out *= transformComponent->transform.scale.z;*/

			/*colliderComponent->NarrowPhase.p1 = right + out;
			colliderComponent->NarrowPhase.p2 = right + (-1.0f * out);
			colliderComponent->NarrowPhase.p3 = (-1.0f * right) + out;
			colliderComponent->NarrowPhase.p4 = (-1.0f * right) + (-1.0f * out);*/

			// Rotate
			colliderComponent->NarrowPhase.p1 = colliderComponent->NarrowPhase.p1 * transformComponent->transform.rotation;
			colliderComponent->NarrowPhase.p2 = colliderComponent->NarrowPhase.p2 * transformComponent->transform.rotation;
			colliderComponent->NarrowPhase.p3 = colliderComponent->NarrowPhase.p3 * transformComponent->transform.rotation;
			colliderComponent->NarrowPhase.p4 = colliderComponent->NarrowPhase.p4 * transformComponent->transform.rotation;

			// Rotate Normal
			up = transformComponent->transform.rotation * up;
			colliderComponent->NarrowPhase.normal = glm::normalize(up);

			colliderComponent->NarrowPhase.magnitude = glm::dot(colliderComponent->NarrowPhase.normal, transformComponent->transform.position);

			// Rotate Plane boundaries
			right = transformComponent->transform.rotation * right;
			out = transformComponent->transform.rotation * out;

			// Scale 
			right *= transformComponent->transform.scale.x;
			out *= transformComponent->transform.scale.z;

			// Assign Boundaries
			colliderComponent->NarrowPhase.p1 = right + out;
			colliderComponent->NarrowPhase.p2 = right + (-1.0f * out);
			colliderComponent->NarrowPhase.p3 = (-1.0f * right) + out;
			colliderComponent->NarrowPhase.p4 = (-1.0f * right) + (-1.0f * out);

			// Translate
			colliderComponent->NarrowPhase.p1 = colliderComponent->NarrowPhase.p1 + transformComponent->transform.position;
			colliderComponent->NarrowPhase.p2 = colliderComponent->NarrowPhase.p2 + transformComponent->transform.position;
			colliderComponent->NarrowPhase.p3 = colliderComponent->NarrowPhase.p3 + transformComponent->transform.position;
			colliderComponent->NarrowPhase.p4 = colliderComponent->NarrowPhase.p4 + transformComponent->transform.position;

		}
		else if (colliderComponent->NarrowPhase.shape == Shape::AABB)
		{

		}
	}
}

void PhysicsSystem::UpdateColliders()
{
	for (const auto& [colliderEntity, colliderComponent] : ColliderComponentPool.componentList)
	{
		UpdateSingleCollider(colliderEntity);	
	}
}

void PhysicsSystem::UpdateMovingColliders()
{
	for (const auto& [colliderEntity, movingBodyComponent] : MovingBodyComponentPool.componentList)
	{
		/*if (rigidBodyComponent->bodyType == Body_Type::STATIC)
			continue;*/
		auto transformComponent = TransformComponentPool.GetComponentByEntity(colliderEntity);
		auto colliderComponent = ColliderComponentPool.GetComponentByEntity(colliderEntity);
		colliderComponent->setPostion(transformComponent->transform.position);
	}
}

/*
 Static Collision Functions :
	int StaticSphereToStaticSphere(glm::vec3* pCenter0, float Radius0, glm::vec3* pCenter1, float Radius1)
	int StaticSphereToStacticPlane(glm::vec3* pCenter, float Radius, glm::vec3* normal, float magnitude, float& distanceFromSphereCenter)
	int StaticAABBToStaticAABB(glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* p2, glm::vec3* max2, glm::vec3* min2)
	int StaticSphereToStaticAABB(glm::vec3* pCenter0, float Radius0, glm::vec3* p1, glm::vec3* max1, glm::vec3* min1)
	int StaticAABBToStaticPlane(glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* normal, float magnitude)
*/

bool PhysicsSystem::CheckCollision(ColliderComponent* c1, ColliderComponent* c2)
{
	bool out = false;
	switch (c1->NarrowPhase.shape)
	{
	case (Shape::SPHERE):
	{
		switch (c2->NarrowPhase.shape)
		{
		case (Shape::SPHERE):
		{
			if (StaticSphereToStaticSphere(&c1->position, c1->NarrowPhase.radius, &c2->position, c2->NarrowPhase.radius) != 0)
				out = true;
			break;
		}
		case (Shape::AABB):
		{
			if (StaticSphereToStaticAABB(&c1->position, c1->NarrowPhase.radius, &c2->position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint) != 0)
				out = true;
			break;
		}
		case (Shape::PLANE):
		{
			float temp = 0.0;
			if (StaticSphereToStacticPlane(&c1->position, c1->NarrowPhase.radius, &c2->NarrowPhase.normal, c2->NarrowPhase.magnitude, temp) != 0)
				out = true;
			break;
		}
		default:
			break;
		}
		return out;
		break;
	}
	case (Shape::AABB):
	{
		switch (c2->NarrowPhase.shape)
		{
		case (Shape::SPHERE):
		{
			if (StaticSphereToStaticAABB(&c2->position, c2->NarrowPhase.radius, &c1->position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint) != 0)
				out = true;
			break;
		}
		case (Shape::AABB):
		{
			if (StaticAABBToStaticAABB(&c1->position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint, &c2->position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint) != 0)
				out = true;
			break;
		}
		case (Shape::PLANE):
		{
			if (StaticAABBToStaticPlane(&c1->position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint, &c2->NarrowPhase.normal, c2->NarrowPhase.magnitude) != 0)
				out = true;
			break;
		}
		default:
			break;
		}
		return out;
		break;
	}
	case (Shape::PLANE):
	{
		switch (c2->NarrowPhase.shape)
		{
		case (Shape::SPHERE):
		{
			float temp = 0.0;
			if (StaticSphereToStacticPlane(&c2->position, c2->NarrowPhase.radius, &c1->NarrowPhase.normal, c1->NarrowPhase.magnitude, temp) != 0)
				out = true;
			break;
		}
		case (Shape::AABB):
		{
			if (StaticAABBToStaticPlane(&c2->position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint, &c1->NarrowPhase.normal, c1->NarrowPhase.magnitude) != 0)
				out = true;
			break;
		}
		default:
			break;
		}
		return out;
	}
	default:
		break;
	}

	return out;
}