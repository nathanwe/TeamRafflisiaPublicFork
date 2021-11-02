#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H
//#include "pch.h"

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

	void Integrate(MovingBodyComponent* movingBody, float dt);

	void DetectCollision(float dt);


	bool Destroy();
};

#endif // !PHYSICS_SYSTEM_H
