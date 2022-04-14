#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H
#include "pch.h"

#define GRAVITY 2.0f
#include "../Core/System.h"

class MovingBodyComponent;

class PhysicsSystem : public System
{
public:
	// default constructor
	PhysicsSystem() = default;

	// delete copy and assignment constructor
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator= (const PhysicsSystem&) = delete;

	bool Init();

	void Update(float timeStamp);
	void UpdatePosition();

	void UpdateSingleCollider(Entity e);
	void UpdateColliders();
	void UpdateMovingColliders();

	void Integrate(MovingBodyComponent* movingBody, float dt);

	

	void DetectCollision(float dt);
	bool CheckCollision(ColliderComponent* c1, ColliderComponent* c2);


	bool Destroy();
private:
	void SendEvent(Entity e1, Entity e2, float dt);
};

#endif // !PHYSICS_SYSTEM_H
