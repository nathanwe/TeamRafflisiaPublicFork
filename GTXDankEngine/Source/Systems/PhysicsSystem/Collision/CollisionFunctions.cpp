#include "pch.h"
#include "CollisionFunctions.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"


//
int StaticPointToStaticSphere(glm::vec3* pP, glm::vec3* pCenter, float Radius)
{
	glm::vec3 pointToCenter = *pP - *pCenter;

	//if ((Radius * Radius) < ((pointToCenter.x * pointToCenter.x) + (pointToCenter.y * pointToCenter.y) + (pointToCenter.z * pointToCenter.z)))
	if ((Radius * Radius) < glm::dot(pointToCenter, pointToCenter))
	{
		return 0;
	}
	else
	{
		return 1;
	}
	return 0;
}



int StaticSphereToStaticSphere(glm::vec3* pCenter0, float Radius0, glm::vec3* pCenter1, float Radius1)
{
	glm::vec3 centerToCenter = *pCenter0 - *pCenter1;

	//if ((Radius0 + Radius1) * (Radius0 + Radius1) < ((centerToCenter.x * centerToCenter.x) + (centerToCenter.y * centerToCenter.y) + (centerToCenter.z * centerToCenter.z)))
	if ((Radius0 + Radius1) * (Radius0 + Radius1) < glm::dot(centerToCenter, centerToCenter))
	{
		return 0;
	}
	else
	{
		return 1;
	}
	return 0;
}
//
//
//
int StaticSphereToStacticPlane(glm::vec3* pCenter, float Radius, glm::vec3* normal, float magnitude)
{
	float distanceFromSphereCenter = (float)fabs(glm::dot(*pCenter, *normal) + magnitude);

	if (distanceFromSphereCenter > Radius)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
//
//
float DynamicSphereToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* normal, float magnitude)
{
	//float d = 0;
	//float t = 0;
	float dt = 0;
	//Same Side of Plane
	if (glm::dot(*pCenter0, *normal) * glm::dot(*pCenter0, *normal) > 0)
	{
		return -1;
	}
	//Velocity Parallel to Plane
	else if (glm::dot(*normal, *velocity))
	{
		return -1;
	}

	//Shifting Plane based on Spheres Position
	if (glm::dot(*velocity, *normal) > 0)
	{
		magnitude -= radius;
	}
	else
	{
		magnitude += radius;
	}

	dt = (float)fabs(glm::dot(*pCenter0, *normal) + magnitude) / (float)fabs(glm::dot(*velocity, *normal) + magnitude);

	return dt;
}
//
//
float DynamicPointToStaticSphere(glm::vec3* p1, glm::vec3* p2, glm::vec3* velocity, glm::vec3* pCenter, float Radius)
{
	float pToCenterSq = glm::dot((*pCenter - *p1), (*pCenter - *p1));

	float v = glm::sqrt(glm::dot(*velocity, *velocity));

	if (v == 0)
		return -1;

	float m = glm::dot((*pCenter - *p1), *velocity) / v;

	float nSq = pToCenterSq - (m * m);
	if (m < 0)
	{
		return -1;
	}
	else if (glm::dot((*pCenter - *p1), (*pCenter - *p2)) > 0 && StaticPointToStaticSphere(p2, pCenter, Radius) == 0)
	{
		return -1;
	}
	else if (nSq > (Radius * Radius))
	{
		return -1;
	}

	float s = glm::sqrt((Radius * Radius) - nSq);

	return (m - s) / v;

}
//
//
float DynamicSphereToStaticSphere(glm::vec3* pCenter0i, glm::vec3* pCenter0f, float Radius0, glm::vec3* velocity, glm::vec3* pCenter1, float Radius1)
{

	float dt = DynamicPointToStaticSphere(pCenter0i, pCenter0f, velocity, pCenter1, Radius1 + Radius0);


	/*float pToCenterSq = glm::dot((*pCenter - *p1), (*pCenter - *p1));

	float v = glm::sqrt(glm::dot(*velocity, *velocity));

	float m = glm::dot((*pCenter - *p1), *velocity) / v;

	float nSq = pToCenterSq - (m * m);

	if (m < 0 && pToCenterSq >(Radius * Radius))
	{
		return -1;
	}
	else if (nSq > (Radius * Radius))
	{
		return -1;
	}

	float s = glm::sqrt((Radius * Radius) - nSq);

	return (m - s) / v;*/

	return dt;

}
//
float DynamicSphereToDynamicSphere(glm::vec3* pCenter0i, float Radius0, glm::vec3* velocity0, glm::vec3* pCenter1, float Radius1, glm::vec3* velocity1, float t)
{
	glm::vec3 velocityCombined = *velocity0 - *velocity1;

	glm::vec3 pCenter0f = *pCenter0i + (velocityCombined * t);


	float dt = DynamicSphereToStaticSphere(pCenter0i, &pCenter0f, Radius0, &velocityCombined, pCenter1, Radius1);


	/*float pToCenterSq = glm::dot((*pCenter - *p1), (*pCenter - *p1));

	float v = glm::sqrt(glm::dot(*velocity, *velocity));

	float m = glm::dot((*pCenter - *p1), *velocity) / v;

	float nSq = pToCenterSq - (m * m);

	if (m < 0 && pToCenterSq >(Radius * Radius))
	{
		return -1;
	}
	else if (nSq > (Radius * Radius))
	{
		return -1;
	}

	float s = glm::sqrt((Radius * Radius) - nSq);

	return (m - s) / v;*/

	return dt;

}
//
//
//void ReflectSpherePlane(RigidBodyComponent* rb1, RigidBodyComponent* rb2)
//{
//	if (rb1->bodyType == Body_Type::STATIC && rb2->bodyType == Body_Type::STATIC)
//	{
//		auto t = StaticSphereToStacticPlane(&rb2->position, rb2->collider.radius, &rb1->collider.normal, rb1->collider.magnitude);
//
//		if (t == 0)
//			return;
//
//		float distanceFromSphereCenter = (float)glm::dot(rb2->position, rb1->collider.normal) + rb1->collider.magnitude;
//		float distanceFromSphereCenterMod = (float)fabs(glm::dot(rb2->position, rb1->collider.normal) + rb1->collider.magnitude);
//
//		//Position Adjustment
//		glm::vec3 n = glm::normalize(rb1->collider.normal) * (distanceFromSphereCenter / distanceFromSphereCenterMod) * (rb2->collider.radius - distanceFromSphereCenterMod);
//		rb2->position += n;
//
//		//Velocity Adjustment
//		glm::vec3 v = glm::dot(rb2->velocity, rb1->collider.normal) * glm::normalize(rb1->collider.normal);
//		v *= rb2->elasticity;
//		rb2->velocity -= 2.0f * v;
//	}
//}
//
//
//


void ReflectStaticSphereStaticSphere(MovingBodyComponent* mb1, StillBodyComponent* sb2)
{
	float delT = DynamicSphereToStaticSphere(&mb1->rigidBody.prevPosition, &mb1->rigidBody.position, mb1->BroadPhase.radius, &mb1->rigidBody.velocity, &sb2->position, sb2->BroadPhase.radius);

	if (delT == -1)
		return;

	glm::vec3 contactPostion = mb1->rigidBody.prevPosition + mb1->rigidBody.velocity * delT;

	// Incident direction - opposite
	glm::vec3 m = mb1->rigidBody.prevPosition - contactPostion;

	// Normal
	glm::vec3 n = contactPostion - sb2->position;
	// Unit vector
	n = glm::normalize(n);

	// Reflected Vector
	glm::vec3 r = (2 * (glm::dot(m, n)) * n) - m;

	
	mb1->rigidBody.position = contactPostion + r;
	if (mb1->rigidBody.elasticity == 0)
	{
		mb1->rigidBody.velocity -= glm::dot(n, mb1->rigidBody.velocity) * n;
	}
	else
	{
		mb1->rigidBody.velocity -= 2 * glm::dot(n, mb1->rigidBody.velocity) * n;
		mb1->rigidBody.velocity *= (mb1->rigidBody.elasticity + 1) / 2;
	}
}


void ReflectMovingSphereMovingSphere(MovingBodyComponent* mb1, MovingBodyComponent* mb2, float dt)
{
	glm::vec3 V1, vNorm1;
	glm::vec3 V2, vNorm2;
	float delT = DynamicSphereToDynamicSphere(&mb1->rigidBody.prevPosition, mb1->BroadPhase.radius, &mb1->rigidBody.velocity, &mb2->rigidBody.prevPosition, mb2->BroadPhase.radius, &mb2->rigidBody.velocity, dt);

	if (delT == -1)
		return;

	glm::vec3 contactPostion1 = mb1->rigidBody.prevPosition + mb1->rigidBody.velocity * delT;
	glm::vec3 contactPostion2 = mb2->rigidBody.prevPosition + mb2->rigidBody.velocity * delT;

	glm::vec3 normal = glm::normalize(contactPostion2 - contactPostion1);


	vNorm1 = glm::dot(normal, mb1->rigidBody.velocity) * normal;
	vNorm2 = glm::dot(-normal, mb2->rigidBody.velocity) * normal;

	if (mb1->rigidBody.elasticity + mb2->rigidBody.elasticity == 0)
	{
		V1 = (vNorm1 + vNorm2) / 2.0f;
		V2 = (vNorm1 + vNorm2) / 2.0f;
	}
	else
	{
		V1 = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		V2 = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);


		V1 *= (mb1->rigidBody.elasticity + mb2->rigidBody.elasticity) / 2;
		V2 *= (mb1->rigidBody.elasticity + mb2->rigidBody.elasticity) / 2;
	}
	

	mb1->rigidBody.position = contactPostion1 + V1 * (dt - delT);
	mb2->rigidBody.position = contactPostion2 + V2 * (dt - delT);

	mb1->rigidBody.velocity = V1;
	mb2->rigidBody.velocity = V2;

}