#include "pch.h"
#include "CollisionFunctions.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/ColliderComponent.h"



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


int StaticAABBToStaticAABB(glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* p2, glm::vec3* max2, glm::vec3* min2)
{
	if (p1->x > p2->x && (p1->x + min1->x) >= (p2->x + max2->x))
	{
		return 0;
	}
	else if (p1->x < p2->x && (p1->x + max1->x) <= (p2->x + min2->x))
	{
		return 0;
	}
	else if (p1->y > p2->y && (p1->y + min1->y) >= (p2->y + max2->y))
	{
		return 0;
	}
	else if (p1->y < p2->y && (p1->y + max1->y) <= (p2->y + min2->y))
	{
		return 0;
	}
	else if (p1->z > p2->z && (p1->z + min1->z) >= (p2->z + max2->z))
	{
		return 0;
	}
	else if (p1->z < p2->z && (p1->z + max1->z) <= (p2->z + min2->z))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int StaticSphereToStaticAABB(glm::vec3* pCenter0, float Radius0, glm::vec3* p1, glm::vec3* max1, glm::vec3* min1)
{

	std::vector<glm::vec3> temp;
	glm::vec3 temp1;

	// 1,1,1
	temp.push_back(*p1 + *max1);
	// -1,-1,-1
	temp.push_back(*p1 + *min1);

	// 1,1,-1
	temp1.x = p1->x + max1->x;
	temp1.x = p1->y + max1->y;
	temp1.x = p1->z + min1->z;
	temp.push_back(temp1);

	// 1,-1,1
	temp1.x = p1->x + max1->x;
	temp1.x = p1->y + min1->y;
	temp1.x = p1->z + max1->z;
	temp.push_back(temp1);

	// -1,1,1
	temp1.x = p1->x + min1->x;
	temp1.x = p1->y + max1->y;
	temp1.x = p1->z + max1->z;
	temp.push_back(temp1);

	// 1,-1,-1
	temp1.x = p1->x + max1->x;
	temp1.x = p1->y + min1->y;
	temp1.x = p1->z + min1->z;
	temp.push_back(temp1);

	// -1,-1,1
	temp1.x = p1->x + min1->x;
	temp1.x = p1->y + min1->y;
	temp1.x = p1->z + max1->z;
	temp.push_back(temp1);

	// -1,1,-1
	temp1.x = p1->x + min1->x;
	temp1.x = p1->y + max1->y;
	temp1.x = p1->z + min1->z;
	temp.push_back(temp1);

	// Sphere center is 
	if (pCenter0->x - Radius0 > p1->x + max1->x)
	{
		return 0;
	}
	else if (pCenter0->x + Radius0 < p1->x + min1->x)
	{
		return 0;
	}
	else if (pCenter0->y - Radius0 > p1->y + max1->y)
	{
		return 0;
	}
	else if (pCenter0->y + Radius0 < p1->y + min1->y)
	{
		return 0;
	}
	else if (pCenter0->z - Radius0 > p1->z + max1->z)
	{
		return 0;
	}
	else if (pCenter0->z + Radius0 < p1->z + min1->z)
	{
		return 0;
	}
	else if (pCenter0->x + Radius0 < p1->x + max1->x && pCenter0->x + Radius0 > p1->x + min1->x)
	{
		return 1;
	}
	else if (pCenter0->y + Radius0 < p1->y + max1->y && pCenter0->y + Radius0 > p1->y + min1->y)
	{
		return 1;
	}
	else if (pCenter0->z + Radius0 < p1->z + max1->z && pCenter0->z + Radius0 > p1->z + min1->z)
	{
		return 1;
	}
	else
	{
		for (auto i : temp)
		{
			if (StaticPointToStaticSphere(&i, pCenter0, Radius0) == 1)
			{
				return 1;
			}
		}
		return 0;
	}

}

int StaticAABBToStaticPlane(glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* normal, float magnitude)
{
	std::vector<glm::vec3> temp;
	glm::vec3 temp1;
	
	// 1,1,1
	temp.push_back(*p1 + *max1);
	// -1,-1,-1
	temp.push_back(*p1 + *min1);

	// 1,1,-1
	temp1.x = p1->x + max1->x;
	temp1.x = p1->y + max1->y;
	temp1.x = p1->z + min1->z;
	temp.push_back(temp1);

	// 1,-1,1
	temp1.x = p1->x + max1->x;
	temp1.x = p1->y + min1->y;
	temp1.x = p1->z + max1->z;
	temp.push_back(temp1);

	// -1,1,1
	temp1.x = p1->x + min1->x;
	temp1.x = p1->y + max1->y;
	temp1.x = p1->z + max1->z;
	temp.push_back(temp1);

	// 1,-1,-1
	temp1.x = p1->x + max1->x;
	temp1.x = p1->y + min1->y;
	temp1.x = p1->z + min1->z;
	temp.push_back(temp1);

	// -1,-1,1
	temp1.x = p1->x + min1->x;
	temp1.x = p1->y + min1->y;
	temp1.x = p1->z + max1->z;
	temp.push_back(temp1);

	// -1,1,-1
	temp1.x = p1->x + min1->x;
	temp1.x = p1->y + max1->y;
	temp1.x = p1->z + min1->z;
	temp.push_back(temp1);

	int sign = static_cast<int>(glm::dot(temp[0], *normal) - magnitude / fabs(glm::dot(temp[0], *normal) - magnitude));

	for (auto i : temp)
	{
		if (glm::dot(i, *normal) - magnitude / fabs(glm::dot(temp[0], *normal) - magnitude) != sign)
			return 1;
	}

	return 0;
}




// For RayCast
float DynamicPointToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, glm::vec3* velocity, glm::vec3* normal, float magnitude)
{
	float dt = 0;

	if (((glm::dot(*pCenter0, *normal) - magnitude) * (glm::dot(*pCenter1, *normal) - magnitude)) > 0)
	{
		return -1;
	}
	//Velocity Parallel to Plane
	else if (glm::dot(*normal, *velocity) == 0)
	{
		return -1;
	}
	else
	{
		dt = (float)fabs(glm::dot(*pCenter0, *normal) - magnitude) / (float)fabs(glm::dot(*velocity, *normal));
		return dt;
	}

}
//


float DynamicSphereToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* normal, float magnitude)
{
	//float d = 0;
	//float t = 0;
	float dt = 0;

	//Same Side of Plane
	//auto d1 = glm::dot(*pCenter0, *normal);
	//auto d2 = glm::dot(*pCenter1, *normal);

	//if ((glm::dot(*pCenter0, *normal) * glm::dot(*pCenter1, *normal)) > 0 && fabs(glm::dot(*pCenter1, *normal) > magnitude + radius))
	//{
	//	return -1;
	//}
	////Velocity Parallel to Plane
	//else if (glm::dot(*normal, *velocity))
	//{
	//	return -1;
	//}

	/*if ((glm::dot(*pCenter0, *normal)) - magnitude < radius)
	{
		*pCenter0 += (magnitude - glm::dot(*pCenter0, *normal)) * *normal;
		*pCenter1 += (magnitude - glm::dot(*pCenter0, *normal)) * *normal;
	}*/

	//Shifting Plane based on Spheres Position
	if (glm::dot(*velocity, *normal) > 0)
	{
		magnitude -= radius;
	}
	else
	{
		magnitude += radius;
	}


	dt = DynamicPointToStaticPlane(pCenter0, pCenter1, velocity, normal, magnitude);

	return dt;
}


// For RayCast
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


// For RayCast
//// Intersection Check
//float CheckInt(float dist1, float dist2, glm::vec3* pCenter0, glm::vec3* pCenter1, glm::vec3& Intersection, float t, glm::vec3* velocity) {
//	//dist1, dist2
//	/*float dist1 = *pCenter0 - *point;
//	float dist2 = *pCenter1 - *point;*/
//	if (dist1 * dist2 >= 0.0f) return 0; // Same Side
//	if (dist1 == dist2) return 0;
//	Intersection = *pCenter0 + (*pCenter1 - *pCenter0) * (-dist1 / (dist2 - dist1));
//	return (Intersection.x - pCenter0->x) / velocity->x;
//}
//int inline WithinLimits(glm::vec3& Intersection, glm::vec3* min, glm::vec3* max, const int Axis) {
//	if (Axis == 1 && Intersection.z > min->z && Intersection.z < max->z && Intersection.y > min->y && Intersection.y < max->y) return 1;
//	if (Axis == 2 && Intersection.z > min->z && Intersection.z < max->z && Intersection.x > min->x && Intersection.x < max->x) return 1;
//	if (Axis == 3 && Intersection.x > min->x && Intersection.x < max->x && Intersection.y > min->y && Intersection.y < max->y) return 1;
//	return 0;
//}
//
//float DynamicPointToStaticAABB(glm::vec3* pCenter0, glm::vec3* pCenter1, glm::vec3* velocity, glm::vec3* p, glm::vec3* max, glm::vec3* min, float t)
//{
//	glm::vec3 Intersection;
//	if (pCenter1->x < min->x && pCenter0->x < min->x) return -1;
//	if (pCenter1->x > max->x && pCenter0->x > max->x) return -1;
//	if (pCenter1->y < min->y && pCenter0->y < min->y) return -1;
//	if (pCenter1->y > max->y && pCenter0->y > max->y) return -1;
//	if (pCenter1->z < min->z && pCenter0->z < min->z) return -1;
//	if (pCenter1->z > max->z && pCenter0->z > max->z) return -1;
//	if (pCenter0->x > min->x && pCenter0->x < max->x &&
//		pCenter0->y > min->y && pCenter0->y < max->y &&
//		pCenter0->z > min->z && pCenter0->z < max->z)
//	{
//		return 0;
//		Intersection = *pCenter0; // starting point is within bounding volume
//	}
//	else
//	{
//		if ((CheckInt(pCenter0->x - min->x, pCenter1->x - min->x, pCenter0, pCenter1, Intersection, t, velocity) != 0 && WithinLimits(Intersection, min, max, 1)))
//		{
//			return CheckInt(pCenter0->x - min->x, pCenter1->x - min->x, pCenter0, pCenter1, Intersection, t, velocity);
//		}
//		else if ((CheckInt(pCenter0->y - min->y, pCenter1->y - min->y, pCenter0, pCenter1, Intersection, t, velocity) != 0 && WithinLimits(Intersection, min, max, 2)))
//		{
//			return CheckInt(pCenter0->y - min->y, pCenter1->y - min->y, pCenter0, pCenter1, Intersection, t, velocity);
//		}
//		else if ((CheckInt(pCenter0->z - min->z, pCenter1->z - min->z, pCenter0, pCenter1, Intersection, t, velocity) != 0 && WithinLimits(Intersection, min, max, 3)))
//		{
//			return CheckInt(pCenter0->z - min->z, pCenter1->z - min->z, pCenter0, pCenter1, Intersection, t, velocity);
//		}
//		else if ((CheckInt(pCenter0->x - max->x, pCenter1->x - max->x, pCenter0, pCenter1, Intersection, t, velocity) != 0 && WithinLimits(Intersection, min, max, 1)))
//		{
//			return CheckInt(pCenter0->x - max->x, pCenter1->x - max->x, pCenter0, pCenter1, Intersection, t, velocity);
//		}
//		else if ((CheckInt(pCenter0->y - max->y, pCenter1->y - max->y, pCenter0, pCenter1, Intersection, t, velocity) != 0 && WithinLimits(Intersection, min, max, 2)))
//		{
//			return CheckInt(pCenter0->y - max->y, pCenter1->y - max->y, pCenter0, pCenter1, Intersection, t, velocity);
//		}
//		else if ((CheckInt(pCenter0->z - max->z, pCenter1->z - max->z, pCenter0, pCenter1, Intersection, t, velocity) != 0 && WithinLimits(Intersection, min, max, 3)))
//		{
//			return CheckInt(pCenter0->z - max->z, pCenter1->z - max->z, pCenter0, pCenter1, Intersection, t, velocity);
//		}
//	}
//	return -1;
//}

float DynamicPointToStaticAABB(glm::vec3* pCenter0, glm::vec3* pCenter1, glm::vec3* velocity, glm::vec3* p, glm::vec3* max, glm::vec3* min, float t)
{
	float t1, t2, t3, t4, t5, t6;
	if (velocity->x != 0)
	{
		t1 = (p->x + min->x - pCenter0->x) / velocity->x;
		t2 = (p->x + max->x - pCenter0->x) / velocity->x;
	}
	else
	{
		t1 = INT_MAX;
		t2 = INT_MAX;
	}

	if (velocity->y != 0)
	{
		t3 = (p->y + min->y - pCenter0->y) / velocity->y;
		t4 = (p->y + max->y - pCenter0->y) / velocity->y;
	}
	else
	{
		t3 = INT_MAX;
		t4 = INT_MAX;
	}

	if (velocity->z != 0)
	{
		t5 = (p->z + min->z - pCenter0->z) / velocity->z;
		t6 = (p->z + max->z - pCenter0->z) / velocity->z;
	}
	else
	{
		t5 = INT_MAX;
		t6 = INT_MAX;
	}
	/*t1 = (p->x + min->x - pCenter0->x) / velocity->x;
	t2 = (p->x + max->x - pCenter0->x) / velocity->x;
	t3 = (p->y + min->y - pCenter0->y) / velocity->y;
	t4 = (p->y + max->y - pCenter0->y) / velocity->y;
	t5 = (p->z + min->z - pCenter0->z) / velocity->z;
	t6 = (p->z + max->z - pCenter0->z) / velocity->z;*/

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0) {
		return -1;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		return -1;
	}

	if (tmin < 0.0f) {
		return tmax;
		LOG_INFO(pCenter0->x + velocity->x * tmax);
		LOG_INFO(pCenter0->y + velocity->y * tmax);
		LOG_INFO(pCenter0->z + velocity->z * tmax);
	}


	LOG_INFO(pCenter0->x + velocity->x * tmin);
	LOG_INFO(pCenter0->y + velocity->y * tmin);
	LOG_INFO(pCenter0->z + velocity->z * tmin);
	return tmin;
}


//
//float DynamicSphereToStaticAABB(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* p, glm::vec3* max, glm::vec3* min, float t)
//{
//	glm::vec3 Intersection;
//	if (pCenter1->x < min->x && pCenter1->x + radius < min->x && pCenter0->x < min->x && pCenter0->x + radius < min->x) return -1;
//	if (pCenter1->x > max->x && pCenter1->x - radius > max->x && pCenter0->x > max->x && pCenter0->x - radius > max->x) return -1;
//	if (pCenter1->y < min->y && pCenter1->y + radius < min->y && pCenter0->y < min->y && pCenter0->y + radius < min->y) return -1;
//	if (pCenter1->y > max->y && pCenter1->y - radius > max->y && pCenter0->y > max->y && pCenter0->y - radius > max->y) return -1;
//	if (pCenter1->z < min->z && pCenter1->z + radius < min->z && pCenter0->z < min->z && pCenter0->z + radius < min->z) return -1;
//	if (pCenter1->z > max->z && pCenter1->z - radius > max->z && pCenter0->z > max->z && pCenter0->z - radius > max->z) return -1;
//	if (StaticSphereToStaticAABB(pCenter0, radius, p, max, min))
//	{
//		return 0;
//	}
//
//
//}



bool ReflectMovingSphereStaticPlane(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb2, ColliderComponent* c2, float dt)
{
	auto delT = DynamicSphereToStaticPlane(&mb1->rigidBody.prevPosition, &mb1->rigidBody.position, c1->NarrowPhase.radius, &mb1->rigidBody.velocity, &(c2->NarrowPhase.normal), c2->NarrowPhase.magnitude);

	if (delT == -1)
		return false;

	glm::vec3 contactPostion = mb1->rigidBody.prevPosition + mb1->rigidBody.velocity * delT;


	/*float distanceFromSphereCenter = (float)glm::dot(mb1->rigidBody.position, sb2->BroadPhase.normal) + sb2->BroadPhase.magnitude;
	float distanceFromSphereCenterMod = (float)fabs(glm::dot(mb1->rigidBody.position, sb2->BroadPhase.normal) + sb2->BroadPhase.magnitude);*/



	//Velocity Adjustment
	glm::vec3 v = glm::dot(mb1->rigidBody.velocity, c2->NarrowPhase.normal) * glm::normalize(c2->NarrowPhase.normal);
	v *= (1 + mb1->rigidBody.elasticity) / 2;

	mb1->rigidBody.velocity -= 2.0f * v;

	// Reflected Vector
	glm::vec3 r = mb1->rigidBody.velocity * (dt - delT);

	//Position Adjustment
	//glm::vec3 n = glm::normalize(sb2->BroadPhase.normal) * (distanceFromSphereCenter / distanceFromSphereCenterMod) * (mb1->BroadPhase.radius - distanceFromSphereCenterMod);
	mb1->rigidBody.position = contactPostion + r;

	return true;
}


bool ReflectMovingSphereStaticSphere(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb2, ColliderComponent* c2)
{
	float delT = DynamicSphereToStaticSphere(&mb1->rigidBody.prevPosition, &mb1->rigidBody.position, c1->NarrowPhase.radius, &mb1->rigidBody.velocity, &sb2->position, c2->NarrowPhase.radius);

	if (delT == -1)
		return false;

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

	return true;
}


bool ReflectMovingSphereMovingSphere(MovingBodyComponent* mb1, ColliderComponent* c1, MovingBodyComponent* mb2, ColliderComponent* c2, float dt)
{
	glm::vec3 V1, vNorm1;
	glm::vec3 V2, vNorm2;
	float delT = DynamicSphereToDynamicSphere(&mb1->rigidBody.prevPosition, c1->NarrowPhase.radius, &mb1->rigidBody.velocity, &mb2->rigidBody.prevPosition, c2->NarrowPhase.radius, &mb2->rigidBody.velocity, dt);

	if (delT == -1)
		return false;

	glm::vec3 contactPostion1 = mb1->rigidBody.prevPosition + mb1->rigidBody.velocity * delT;
	glm::vec3 contactPostion2 = mb2->rigidBody.prevPosition + mb2->rigidBody.velocity * delT;

	glm::vec3 normal = glm::normalize(contactPostion2 - contactPostion1);


	vNorm1 = glm::dot(normal, mb1->rigidBody.velocity) * normal;
	vNorm2 = glm::dot(normal, mb2->rigidBody.velocity) * normal;

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
	return true;

}

bool ReflectMovingAABBStaticAABB(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb1, ColliderComponent* c2)
{
	//glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* p2, glm::vec3* max2, glm::vec3* min2
	int CollisionCheck = StaticAABBToStaticAABB(&mb1->rigidBody.position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint, &sb1->position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint);

	if (CollisionCheck == 0)
	{
		return false;
	}

	// Time to exit AABB in 3 axial direction
	float t1 = -1;
	float t2 = -1;
	float t3 = -1;
	
	// Object moving toward right - +ve x
	if (mb1->rigidBody.velocity.x > 0)
	{
		t1 = (mb1->rigidBody.position.x + c1->NarrowPhase.maxPoint.x - (sb1->position.x + c2->NarrowPhase.minPoint.x)) / mb1->rigidBody.velocity.x;
	}
	// Object moving toward left - -ve x
	else
	{
		t1 = (mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x - (sb1->position.x + c2->NarrowPhase.maxPoint.x)) / mb1->rigidBody.velocity.x;
	}

	// Object moving up - +ve y
	if (mb1->rigidBody.velocity.y > 0)
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y - (sb1->position.y + c2->NarrowPhase.minPoint.y)) / mb1->rigidBody.velocity.y;
	}
	// Object moving down - -ve y
	else
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y - (sb1->position.y + c2->NarrowPhase.maxPoint.y)) / mb1->rigidBody.velocity.y;
	}

	// Object moving out of - +ve z
	if (mb1->rigidBody.velocity.z > 0)
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z - (sb1->position.z + c2->NarrowPhase.minPoint.z)) / mb1->rigidBody.velocity.z;
	}
	// Object moving into - -ve z
	else
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z - (sb1->position.z + c2->NarrowPhase.maxPoint.z)) / mb1->rigidBody.velocity.z;
	}


	if (t1 < t2 && t1 < t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;
		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
	}
	else if (t2 < t1 && t2 < t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;
		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
	}
	else if (t3 < t1 && t3 < t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t3;
		mb1->rigidBody.velocity.z = -mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;
	}
	else if (t1 == t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;

		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
	}
	else if (t3 == t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;

		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.z = -mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;
	}
	else if(t1 == t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;

		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.z = -mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;		
	}
	return true;
}
bool ReflectMovingAABBMovingAABB(MovingBodyComponent* mb1, ColliderComponent* c1, MovingBodyComponent* mb2, ColliderComponent* c2)
{
	int CollisionCheck = StaticAABBToStaticAABB(&mb1->rigidBody.position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint, &mb1->rigidBody.position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint);

	if (CollisionCheck == 0)
	{
		return false;
	}


	// Time to exit AABB in 3 axial direction
	float t1 = -1;
	float t2 = -1;
	float t3 = -1;

	glm::vec3 velocity = mb1->rigidBody.velocity - mb2->rigidBody.velocity;

	// Objects as a system moving toward right - +ve x
	if (velocity.x > 0)
	{
		t1 = (mb1->rigidBody.position.x + c1->NarrowPhase.maxPoint.x - (mb2->rigidBody.position.x + c2->NarrowPhase.minPoint.x)) / velocity.x;
	}
	// Object as a system moving toward left - -ve x
	else
	{
		t1 = (mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x - (mb2->rigidBody.position.x + c2->NarrowPhase.maxPoint.x)) / velocity.x;
	}

	// Object as a system moving up - +ve y
	if (velocity.y > 0)
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y - (mb2->rigidBody.position.y + c2->NarrowPhase.minPoint.y)) / velocity.y;
	}
	// Object as a system moving down - -ve y
	else
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y - (mb2->rigidBody.position.y + c2->NarrowPhase.maxPoint.y)) / velocity.y;
	}

	// Object as a system moving out of - +ve z
	if (velocity.z > 0)
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z - (mb2->rigidBody.position.z + c2->NarrowPhase.minPoint.z)) / velocity.z;
	}
	// Object as a system moving into - -ve z
	else
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z - (mb2->rigidBody.position.z + c2->NarrowPhase.maxPoint.z)) / velocity.z;
	}


	if (t1 < t2 && t1 < t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;
		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;

		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t1;
		mb2->rigidBody.velocity.x = -mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;
	}
	else if (t2 < t1 && t2 < t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;
		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;

		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t2;
		mb2->rigidBody.velocity.y = -mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;
	}
	else if (t3 < t1 && t3 < t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t3;
		mb1->rigidBody.velocity.z = -mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;

		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t3;
		mb2->rigidBody.velocity.z = -mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
	}
	else if (t1 == t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;

		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;

		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t1;

		mb2->rigidBody.velocity.x = -mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;
		mb2->rigidBody.velocity.y = -mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;
	}
	else if (t3 == t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;

		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.z = -mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;

		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t2;

		mb2->rigidBody.velocity.y = -mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;
		mb2->rigidBody.velocity.z = -mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
	}
	else if (t1 == t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;

		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.z = -mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;

		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t2;

		mb2->rigidBody.velocity.x = -mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;
		mb2->rigidBody.velocity.z = -mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
	}

	return true;
}

bool ReflectMovingAABBStaticPlane(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb1, ColliderComponent* c2)
{
	int CollisionCheck = StaticAABBToStaticPlane(&mb1->rigidBody.position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint, &c2->NarrowPhase.normal, c2->NarrowPhase.magnitude);
	return false;

	std::vector<glm::vec3> temp;
	glm::vec3 temp1;

	// 1,1,1
	temp.push_back(mb1->rigidBody.position + c1->NarrowPhase.maxPoint);
	// -1,-1,-1
	temp.push_back(mb1->rigidBody.position + c1->NarrowPhase.minPoint);

	// 1,1,-1
	temp1.x = mb1->rigidBody.position.x + c1->NarrowPhase.maxPoint.x;
	temp1.x = mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y;
	temp1.x = mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z;
	temp.push_back(temp1);

	// 1,-1,1
	temp1.x = mb1->rigidBody.position.x + c1->NarrowPhase.maxPoint.x;
	temp1.x = mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y;
	temp1.x = mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z;
	temp.push_back(temp1);

	// -1,1,1
	temp1.x = mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x;
	temp1.x = mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y;
	temp1.x = mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z;
	temp.push_back(temp1);

	// 1,-1,-1
	temp1.x = mb1->rigidBody.position.x + c1->NarrowPhase.maxPoint.x;
	temp1.x = mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y;
	temp1.x = mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z;
	temp.push_back(temp1);

	// -1,-1,1
	temp1.x = mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x;
	temp1.x = mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y;
	temp1.x = mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z;
	temp.push_back(temp1);

	// -1,1,-1
	temp1.x = mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x;
	temp1.x = mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y;
	temp1.x = mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z;
	temp.push_back(temp1);

	float min = 0;
	for (auto i : temp)
	{
		if (glm::dot(i, c2->NarrowPhase.normal) - c2->NarrowPhase.magnitude < min)
			min = glm::dot(i, c2->NarrowPhase.normal);
	}

	if (min < 0)
	{
		mb1->rigidBody.position = mb1->rigidBody.position + (c2->NarrowPhase.normal * min * (-1.0f));
		mb1->rigidBody.velocity -= (glm::dot(c2->NarrowPhase.normal, mb1->rigidBody.velocity)) * c2->NarrowPhase.normal;
		mb1->rigidBody.velocity += (mb1->rigidBody.elasticity * glm::dot(c2->NarrowPhase.normal, mb1->rigidBody.velocity)) * c2->NarrowPhase.normal;
	}
}