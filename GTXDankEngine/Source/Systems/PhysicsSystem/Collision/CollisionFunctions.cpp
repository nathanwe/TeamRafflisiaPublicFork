#include "pch.h"
#include "CollisionFunctions.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/ColliderComponent.h"

#include <limits>


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


int StaticSphereToStacticPlane(glm::vec3* pCenter, float Radius, glm::vec3* normal, float magnitude, float& distanceFromSphereCenter)
{
	distanceFromSphereCenter = (float)fabs(glm::dot(*pCenter, *normal) - magnitude);

	if (distanceFromSphereCenter >= Radius)
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
//
// 0 for no collision
// 1 for exact collision
// 2 for penetration
int StaticSphereToStaticAABB(glm::vec3* pCenter0, float Radius0, glm::vec3* p1, glm::vec3* max1, glm::vec3* min1)
{

	//std::vector<glm::vec3> temp;
	//glm::vec3 temp1;

	//// 1,1,1
	//temp.push_back(*p1 + *max1);
	//// -1,-1,-1
	//temp.push_back(*p1 + *min1);

	//// 1,1,-1
	//temp1.x = p1->x + max1->x;
	//temp1.x = p1->y + max1->y;
	//temp1.x = p1->z + min1->z;
	//temp.push_back(temp1);

	//// 1,-1,1
	//temp1.x = p1->x + max1->x;
	//temp1.x = p1->y + min1->y;
	//temp1.x = p1->z + max1->z;
	//temp.push_back(temp1);

	//// -1,1,1
	//temp1.x = p1->x + min1->x;
	//temp1.x = p1->y + max1->y;
	//temp1.x = p1->z + max1->z;
	//temp.push_back(temp1);

	//// 1,-1,-1
	//temp1.x = p1->x + max1->x;
	//temp1.x = p1->y + min1->y;
	//temp1.x = p1->z + min1->z;
	//temp.push_back(temp1);

	//// -1,-1,1
	//temp1.x = p1->x + min1->x;
	//temp1.x = p1->y + min1->y;
	//temp1.x = p1->z + max1->z;
	//temp.push_back(temp1);

	//// -1,1,-1
	//temp1.x = p1->x + min1->x;
	//temp1.x = p1->y + max1->y;
	//temp1.x = p1->z + min1->z;
	//temp.push_back(temp1);

	//// Sphere center is 
	//if (pCenter0->x > p1->x + max1->x + Radius0)
	//{
	//	return 0;
	//}
	//else if (pCenter0->x < p1->x + min1->x - Radius0)
	//{
	//	return 0;
	//}
	//else if (pCenter0->y > p1->y + max1->y + Radius0)
	//{
	//	return 0;
	//}
	//else if (pCenter0->y < p1->y + min1->y - Radius0)
	//{
	//	return 0;
	//}
	//else if (pCenter0->z > p1->z + max1->z + Radius0)
	//{
	//	return 0;
	//}
	//else if (pCenter0->z < p1->z + min1->z - Radius0)
	//{
	//	return 0;
	//}
	//// XY
	//else if (pCenter0->x < p1->x + max1->x && pCenter0->x > p1->x + min1->x && pCenter0->y < p1->y + max1->y && pCenter0->y > p1->y + min1->y)
	//{
	//	return 1;
	//}
	//// YZ
	//else if (pCenter0->y < p1->y + max1->y && pCenter0->y > p1->y + min1->y && pCenter0->z < p1->z + max1->z && pCenter0->z > p1->z + min1->z)
	//{
	//	return 1;
	//}
	//// ZX
	//else if (pCenter0->z < p1->z + max1->z && pCenter0->z > p1->z + min1->z && pCenter0->x < p1->x + max1->x && pCenter0->x > p1->x + min1->x)
	//{
	//	return 1;
	//}
	//else
	//{
	//	for (auto i : temp)
	//	{
	//		if (StaticPointToStaticSphere(&i, pCenter0, Radius0) == 1)
	//		{
	//			return 1;
	//		}
	//	}
	//	return 0;
	//}

	// Better Approach - Check
	float d = 0;

	// X
	if (pCenter0->x < p1->x + min1->x)
	{
		d = d + (pCenter0->x - (p1->x + min1->x)) * (pCenter0->x - (p1->x + min1->x));
	}
	else if (pCenter0->x > p1->x + max1->x)
	{
		d = d + (pCenter0->x - (p1->x + max1->x)) * (pCenter0->x - (p1->x + max1->x));
	}
	// Y
	if (pCenter0->y < p1->y + min1->y)
	{
		d = d + (pCenter0->y - (p1->y + min1->y)) * (pCenter0->y - (p1->y + min1->y));
	}
	else if (pCenter0->y > p1->y + max1->y)
	{
		d = d + (pCenter0->y - (p1->y + max1->y)) * (pCenter0->y - (p1->y + max1->y));
	}
	// Z
	if (pCenter0->z < p1->z + min1->z)
	{
		d = d + (pCenter0->z - (p1->z + min1->z)) * (pCenter0->z - (p1->z + min1->z));
	}
	else if (pCenter0->z > p1->z + max1->z)
	{
		d = d + (pCenter0->z - (p1->z + max1->z)) * (pCenter0->z - (p1->z + max1->z));
	}

	if (d > Radius0 * Radius0)
	{
		return 0;
	}
	else if(d == Radius0 * Radius0)
	{
		return 1;
	}
	else
	{
		return 2;
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
float DynamicSphereToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* p0, glm::vec3* normal, float magnitude, glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* p4, float t)
{
	float dt = -1;
	float distanceFromSphereCenter;
	if (StaticSphereToStacticPlane(pCenter0, radius, normal, magnitude, distanceFromSphereCenter))
	{
		*pCenter0 += *normal * (radius - distanceFromSphereCenter);
		//*pCenter1 = *pCenter0 + *velocity * dt;
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


	dt = DynamicPointToStaticPlane(pCenter0, pCenter1, velocity, normal, magnitude);

	if (dt == -1)
		return -1;

	glm::vec3 pNew = *pCenter0 + *velocity * dt;
	glm::vec3 pointOfContact = pNew;
	if (glm::dot(*velocity, *normal) - magnitude > 0)
	{
		pointOfContact = pointOfContact + *normal * -1.0f * (glm::dot(pointOfContact, *normal) - magnitude);
	}
	else
	{
		pointOfContact = pointOfContact + *normal * (glm::dot(pointOfContact, *normal) - magnitude);
	}

	/*glm::vec3 p1 = *p0 + *right + *out;
	glm::vec3 p2 = *p0 + *right - *out;
	glm::vec3 p3 = *p0 - *right + *out;
	glm::vec3 p4 = *p0 - *right - *out;*/

	
	//glm::vec2 pProj1, pProj2, pProj3, pProj4;

	// Project along X - YZ plane
	if (fabs(normal->x) > fabs(normal->y) && fabs(normal->x) > fabs(normal->z))
	{
		/*pProj1.x = p1.y;
		pProj2.x = p2.y;
		pProj3.x = p3.y;
		pProj4.x = p4.y;

		pProj1.y = p1.z;
		pProj2.y = p2.z;
		pProj3.y = p3.z;
		pProj4.y = p4.z;*/

		if (pointOfContact.y > std::max(std::max(p1->y, p2->y), std::max(p3->y, p4->y)) || pointOfContact.z > std::max(std::max(p1->z, p2->z), std::max(p3->z, p4->z)) ||
			pointOfContact.y < std::min(std::min(p1->y, p2->y), std::min(p3->y, p4->y)) || pointOfContact.z < std::min(std::min(p1->z, p2->z), std::min(p3->z, p4->z)))
		{
			return -1;
		}
	}
	// Project along Y - XZ plane
	else if (fabs(normal->y) > fabs(normal->z))
	{
		/*pProj1.x = p1.x;
		pProj2.x = p2.x;
		pProj3.x = p3.x;
		pProj4.x = p4.x;

		pProj1.y = p1.z;
		pProj2.y = p2.z;
		pProj3.y = p3.z;
		pProj4.y = p4.z;*/

		if (pointOfContact.x > std::max(std::max(p1->x, p2->x), std::max(p3->x, p4->x)) || pointOfContact.z > std::max(std::max(p1->z, p2->z), std::max(p3->z, p4->z)) ||
			pointOfContact.x < std::min(std::min(p1->x, p2->x), std::min(p3->x, p4->x)) || pointOfContact.z < std::min(std::min(p1->z, p2->z), std::min(p3->z, p4->z)))
		{
			return -1;
		}
	}
	// Project along Z - XY plane
	else
	{
		/*pProj1.x = p1.x;
		pProj2.x = p2.x;
		pProj3.x = p3.x;
		pProj4.x = p4.x;

		pProj1.y = p1.y;
		pProj2.y = p2.y;
		pProj3.y = p3.y;
		pProj4.y = p4.y;*/

		if (pointOfContact.x > std::max(std::max(p1->x, p2->x), std::max(p3->x, p4->x)) || pointOfContact.y > std::max(std::max(p1->y, p2->y), std::max(p3->y, p4->y)) ||
			pointOfContact.x < std::min(std::min(p1->x, p2->x), std::min(p3->x, p4->x)) || pointOfContact.y < std::min(std::min(p1->y, p2->y), std::min(p3->y, p4->y)))
		{
			return -1;
		}
	}

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


float DynamicSphereToStaticSphere(glm::vec3* pCenter0i, glm::vec3* pCenter0f, float Radius0, glm::vec3* velocity, glm::vec3* pCenter1, float Radius1, float t)
{
	if (StaticSphereToStaticSphere(pCenter0i, Radius0, pCenter1, Radius1))
	{
		glm::vec3 correction = glm::normalize(*pCenter0i - *pCenter1) * (Radius0 + Radius1);

		*pCenter0i = *pCenter1 + correction;
		//*pCenter0f = *pCenter0i + *velocity * t;
	}

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


	float dt = DynamicSphereToStaticSphere(pCenter0i, &pCenter0f, Radius0, &velocityCombined, pCenter1, Radius1, t);


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
		t1 = std::numeric_limits<float>::max();
		t2 = std::numeric_limits<float>::max();
	}

	if (velocity->y != 0)
	{
		t3 = (p->y + min->y - pCenter0->y) / velocity->y;
		t4 = (p->y + max->y - pCenter0->y) / velocity->y;
	}
	else
	{
		t3 = std::numeric_limits<float>::max();
		t4 = std::numeric_limits<float>::max();
	}

	if (velocity->z != 0)
	{
		t5 = (p->z + min->z - pCenter0->z) / velocity->z;
		t6 = (p->z + max->z - pCenter0->z) / velocity->z;
	}
	else
	{
		t5 = std::numeric_limits<float>::max();
		t6 = std::numeric_limits<float>::max();
	}
	/*t1 = (p->x + min->x - pCenter0->x) / velocity->x;
	t2 = (p->x + max->x - pCenter0->x) / velocity->x;
	t3 = (p->y + min->y - pCenter0->y) / velocity->y;
	t4 = (p->y + max->y - pCenter0->y) / velocity->y;
	t5 = (p->z + min->z - pCenter0->z) / velocity->z;
	t6 = (p->z + max->z - pCenter0->z) / velocity->z;*/

	float tmin, tmax;

	if (t1 == std::numeric_limits<float>::max() && t3 == std::numeric_limits<float>::max() && t5 == std::numeric_limits<float>::max())
	{
		return -1;
	}
	else if (t1 == std::numeric_limits<float>::max() && t3 == std::numeric_limits<float>::max())
	{
		tmin = std::min(t5, t6);
		tmax = std::max(t5, t6);
	}
	else if (t3 == std::numeric_limits<float>::max() && t5 == std::numeric_limits<float>::max())
	{
		tmin = std::min(t1, t2);
		tmax = std::max(t1, t2);
	}
	else if (t1 == std::numeric_limits<float>::max() && t5 == std::numeric_limits<float>::max())
	{
		tmin = std::min(t3, t4);
		tmax = std::max(t3, t4);
	}
	else if (t1 == std::numeric_limits<float>::max())
	{
		tmin = std::max(std::min(t3, t4), std::min(t5, t6));
		tmax = std::min(std::max(t3, t4), std::max(t5, t6));
	}
	else if (t3 == std::numeric_limits<float>::max())
	{
		tmin = std::max(std::min(t1, t2), std::min(t5, t6));
		tmax = std::min(std::max(t1, t2), std::max(t5, t6));
	}
	else if (t5 == std::numeric_limits<float>::max())
	{
		tmin = std::max(std::min(t1, t2), std::min(t3, t4));
		tmax = std::min(std::max(t1, t2), std::max(t3, t4));
	}
	else
	{
		tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
	}

	/*float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));*/

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
float DynamicSphereToStaticAABB(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* p, glm::vec3* max, glm::vec3* min, float t, glm::vec3* normal)
{
	//glm::vec3 Intersection;
	if (pCenter1->x < p->x + min->x && pCenter1->x + radius < p->x + min->x && pCenter0->x < p->x + min->x && pCenter0->x + radius < p->x + min->x) return -1;
	if (pCenter1->x > p->x + max->x && pCenter1->x - radius > p->x + max->x && pCenter0->x > p->x + max->x && pCenter0->x - radius > p->x + max->x) return -1;
	if (pCenter1->y < p->y + min->y && pCenter1->y + radius < p->y + min->y && pCenter0->y < p->y + min->y && pCenter0->y + radius < p->y + min->y) return -1;
	if (pCenter1->y > p->y + max->y && pCenter1->y - radius > p->y + max->y && pCenter0->y > p->y + max->y && pCenter0->y - radius > p->y + max->y) return -1;
	if (pCenter1->z < p->z + min->z && pCenter1->z + radius < p->z + min->z && pCenter0->z < p->z + min->z && pCenter0->z + radius < p->z + min->z) return -1;
	if (pCenter1->z > p->z + max->z && pCenter1->z - radius > p->z + max->z && pCenter0->z > p->z + max->z && pCenter0->z - radius > p->z + max->z) return -1;

	glm::vec3 minNew;
	minNew.x = min->x - radius;
	minNew.y = min->y - radius;
	minNew.z = min->z - radius;

	glm::vec3 maxNew;
	maxNew.x = max->x + radius;
	maxNew.y = max->y + radius;
	maxNew.z = max->z + radius;

	float t1 = 0, t2 = 0, t3 = 0;
	glm::vec3 correction = -(*velocity);

	LOG_INFO(correction.y);

	// Initial position in intersecting the AABB / Correct it to move to 
	if (StaticSphereToStaticAABB(pCenter0, radius, p, max, min) > 0)
	{
		// Check X Velocity and calculate time to reach x min or max
		if (correction.x == 0)
		{
			t1 = std::numeric_limits<float>::max();
		}
		else
		{
			if ((maxNew.x - pCenter0->x) * correction.x > 0)
			{
				t1 = (maxNew.x - pCenter0->x) / correction.x;
			}
			else if ((minNew.x - pCenter0->x) * correction.x > 0)
			{
				t1 = (minNew.x - pCenter0->x) / correction.x;
			}
		}

		// Check Y Velocity and calculate time to reach y min or max
		if (correction.y == 0)
		{
			t2 = std::numeric_limits<float>::max();
		}
		else
		{
			if ((maxNew.y - pCenter0->y) * correction.y > 0)
			{
				t2 = (maxNew.y - pCenter0->y) / correction.y;
			}
			else if ((minNew.y - pCenter0->y) * correction.y > 0)
			{
				t2 = (minNew.y - pCenter0->y) / correction.y;
			}
		}

		// Check Z Velocity and calculate time to reach z min or max
		if (correction.z == 0)
		{
			t3 = std::numeric_limits<float>::max();
		}
		else
		{
			if ((maxNew.z - pCenter0->z) * correction.z > 0)
			{
				t3 = (maxNew.z - pCenter0->z) / correction.z;
			}
			else if ((minNew.z - pCenter0->z) * correction.z > 0)
			{
				t3 = (minNew.z - pCenter0->z) / correction.z;
			}
		}

		if (t1 < t2 && t1 < t3)
		{
			*pCenter0 += t1 * correction;
		}
		else if (t2 < t3)
		{
			*pCenter0 += t2 * correction;
		}
		else
		{
			*pCenter0 += t3 * correction;
		}

		//*pCenter1 = *pCenter0 + *velocity * t;
	}

	float tNew = DynamicPointToStaticAABB(pCenter0, pCenter1, velocity, p, &maxNew, &minNew, t);

	if (tNew == -1)
		return -1;

	if (tNew > t)
		return -1;

	glm::vec3 pNew;
	pNew.x = pCenter0->x + tNew * velocity->x;
	pNew.y = pCenter0->y + tNew * velocity->y;
	pNew.z = pCenter0->z + tNew * velocity->z;

	
	int u = 0, v = 0;

	glm::vec3 pointOfContact = *p;
	glm::vec3 p1, p2;
	float temp;

	if (pNew.x < p->x + min->x)
	{
		u |= 1;
		pointOfContact.x += min->x;
		*normal = glm::normalize(glm::vec3(-1,0,0));
	}
	if (pNew.x > p->x + max->x)
	{
		v |= 1;
		pointOfContact.x += max->x;
		*normal = glm::normalize(glm::vec3(1, 0, 0));
	}
	if (pNew.y < p->y + min->y)
	{
		u |= 2;
		pointOfContact.y += min->y;
		*normal = glm::normalize(glm::vec3(0, -1, 0));
	}
	if (pNew.y > p->y + max->y)
	{
		v |= 2;
		pointOfContact.y += max->y;
		*normal = glm::normalize(glm::vec3(0, 1, 0));
	}
	if (pNew.z < p->z + min->z) 
	{ 
		u |= 4;
		pointOfContact.z += min->z;
		*normal = glm::normalize(glm::vec3(0, 0, -1));
	}
	if (pNew.z > p->z + max->z)
	{
		v |= 4;
		pointOfContact.z += max->z;
		*normal = glm::normalize(glm::vec3(0, 0, 1));
	}

	int m = u + v;


	if ((m & (m - 1)) == 0)
	{
		return tNew;
	}
	else if(m == 7)
	{
		*normal = glm::normalize(pNew - pointOfContact);
		return tNew;
	}
	else
	{
		if (pointOfContact.x == p->x)
		{
			p1 = pointOfContact;
			p1.x += max->x;
			p2 = pointOfContact;
			p2.x += min->x;

			ClosestPtPointSegment(&pNew, &p1, &p2, temp, pointOfContact);
			*normal = glm::normalize(pNew - pointOfContact);
			return tNew;
		}
		else if (pointOfContact.y == p->y)
		{
			p1 = pointOfContact;
			p1.y += max->y;
			p2 = pointOfContact;
			p2.y += min->y;

			ClosestPtPointSegment(&pNew, &p1, &p2, temp, pointOfContact);
			*normal = glm::normalize(pNew - pointOfContact);
			return tNew;
		}
		else
		{
			p1 = pointOfContact;
			p1.z += max->z;
			p2 = pointOfContact;
			p2.z += min->z;

			ClosestPtPointSegment(&pNew, &p1, &p2, temp, pointOfContact);
			*normal = glm::normalize(pNew - pointOfContact);
			return tNew;
		}
	}
}



bool ReflectMovingSphereStaticPlane(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb2, ColliderComponent* c2, float dt)
{
	auto delT = DynamicSphereToStaticPlane(&mb1->rigidBody.prevPosition, &mb1->rigidBody.position, c1->NarrowPhase.radius, &mb1->rigidBody.velocity, &sb2->position, &(c2->NarrowPhase.normal), c2->NarrowPhase.magnitude, &c2->NarrowPhase.p1, &c2->NarrowPhase.p2, &c2->NarrowPhase.p3, &c2->NarrowPhase.p4, dt);

	if (delT == -1)
		return false;

	glm::vec3 contactPostion = mb1->rigidBody.prevPosition + mb1->rigidBody.velocity * delT;
	/*if(velocity)
	glm::vec3 contactPostionPlane = mb1->rigidBody.prevPosition + mb1->rigidBody.velocity * delT;*/


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


bool ReflectMovingSphereStaticSphere(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb2, ColliderComponent* c2, float dt)
{
	float delT = DynamicSphereToStaticSphere(&mb1->rigidBody.prevPosition, &mb1->rigidBody.position, c1->NarrowPhase.radius, &mb1->rigidBody.velocity, &sb2->position, c2->NarrowPhase.radius, dt);

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
	else if (mb1->rigidBody.velocity.x < 0)
	{
		t1 = (mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x - (sb1->position.x + c2->NarrowPhase.maxPoint.x)) / mb1->rigidBody.velocity.x;
	}
	else
	{
		t1 = std::numeric_limits<float>::max();
	}

	// Object moving up - +ve y
	if (mb1->rigidBody.velocity.y > 0)
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y - (sb1->position.y + c2->NarrowPhase.minPoint.y)) / mb1->rigidBody.velocity.y;
	}
	// Object moving down - -ve y
	else if (mb1->rigidBody.velocity.y < 0)
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y - (sb1->position.y + c2->NarrowPhase.maxPoint.y)) / mb1->rigidBody.velocity.y;
	}
	else
	{
		t2 = std::numeric_limits<float>::max();
	}

	// Object moving out of - +ve z
	if (mb1->rigidBody.velocity.z > 0)
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z - (sb1->position.z + c2->NarrowPhase.minPoint.z)) / mb1->rigidBody.velocity.z;
	}
	// Object moving into - -ve z
	else if (mb1->rigidBody.velocity.z < 0)
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z - (sb1->position.z + c2->NarrowPhase.maxPoint.z)) / mb1->rigidBody.velocity.z;
	}
	else
	{
		t3 = std::numeric_limits<float>::max();
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
	else if (t1 == t2 && t2 == t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;

		mb1->rigidBody.velocity.x = -mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb1->rigidBody.velocity.y = -mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
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
	int CollisionCheck = StaticAABBToStaticAABB(&mb1->rigidBody.position, &c1->NarrowPhase.maxPoint, &c1->NarrowPhase.minPoint, &mb2->rigidBody.position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint);

	float vNorm1 = 0;
	float vNorm2 = 0;

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
	else if (velocity.x < 0)
	{
		t1 = (mb1->rigidBody.position.x + c1->NarrowPhase.minPoint.x - (mb2->rigidBody.position.x + c2->NarrowPhase.maxPoint.x)) / velocity.x;
	}
	else
	{
		t1 = std::numeric_limits<float>::max();
	}

	// Object as a system moving up - +ve y
	if (velocity.y > 0)
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.maxPoint.y - (mb2->rigidBody.position.y + c2->NarrowPhase.minPoint.y)) / velocity.y;
	}
	// Object as a system moving down - -ve y
	else if (velocity.y < 0)
	{
		t2 = (mb1->rigidBody.position.y + c1->NarrowPhase.minPoint.y - (mb2->rigidBody.position.y + c2->NarrowPhase.maxPoint.y)) / velocity.y;
	}
	else
	{
		t2 = std::numeric_limits<float>::max();
	}

	// Object as a system moving out of - +ve z
	if (velocity.z > 0)
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.maxPoint.z - (mb2->rigidBody.position.z + c2->NarrowPhase.minPoint.z)) / velocity.z;
	}
	// Object as a system moving into - -ve z
	else if (velocity.z < 0)
	{
		t3 = (mb1->rigidBody.position.z + c1->NarrowPhase.minPoint.z - (mb2->rigidBody.position.z + c2->NarrowPhase.maxPoint.z)) / velocity.z;
	}
	else
	{
		t3 = std::numeric_limits<float>::max();
	}


	if (t1 < t2 && t1 < t3)
	{
		//Position
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t1;

		//Velocity
		vNorm1 = mb1->rigidBody.velocity.x;
		vNorm2 = mb2->rigidBody.velocity.x;

		mb1->rigidBody.velocity.x = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.x = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.x = mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.x = mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;
	}
	else if (t2 < t1 && t2 < t3)
	{
		//Position
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t2;

		//Velocity
		vNorm1 = mb1->rigidBody.velocity.y;
		vNorm2 = mb2->rigidBody.velocity.y;

		mb1->rigidBody.velocity.y = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.y = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.y = mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.y = mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;
	}
	else if (t3 < t1 && t3 < t2)
	{
		//Position
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t3;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t3;

		//Velocity
		vNorm1 = mb1->rigidBody.velocity.z;
		vNorm2 = mb2->rigidBody.velocity.z;

		mb1->rigidBody.velocity.z = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.z = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.z = mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.z = mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
	}
	else if (t1 == t2 && t2 == t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t1;

		//Velocity - x
		vNorm1 = mb1->rigidBody.velocity.x;
		vNorm2 = mb2->rigidBody.velocity.x;

		mb1->rigidBody.velocity.x = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.x = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.x = mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.x = mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;

		//Velocity - y
		vNorm1 = mb1->rigidBody.velocity.y;
		vNorm2 = mb2->rigidBody.velocity.y;

		mb1->rigidBody.velocity.y = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.y = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.y = mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.y = mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;

		//Velocity - z
		vNorm1 = mb1->rigidBody.velocity.z;
		vNorm2 = mb2->rigidBody.velocity.z;

		mb1->rigidBody.velocity.z = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.z = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.z = mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.z = mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
	}
	else if (t1 == t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t1;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t1;

		//Velocity - x
		vNorm1 = mb1->rigidBody.velocity.x;
		vNorm2 = mb2->rigidBody.velocity.x;

		mb1->rigidBody.velocity.x = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.x = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.x = mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.x = mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;

		//Velocity - y
		vNorm1 = mb1->rigidBody.velocity.y;
		vNorm2 = mb2->rigidBody.velocity.y;

		mb1->rigidBody.velocity.y = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.y = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.y = mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.y = mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;
	}
	else if (t3 == t2)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t2;

		//Velocity - y
		vNorm1 = mb1->rigidBody.velocity.y;
		vNorm2 = mb2->rigidBody.velocity.y;

		mb1->rigidBody.velocity.y = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.y = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.y = mb1->rigidBody.velocity.y * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.y = mb2->rigidBody.velocity.y * mb2->rigidBody.elasticity;

		//Velocity - z
		vNorm1 = mb1->rigidBody.velocity.z;
		vNorm2 = mb2->rigidBody.velocity.z;

		mb1->rigidBody.velocity.z = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.z = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.z = mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.z = mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
	}
	else if (t1 == t3)
	{
		mb1->rigidBody.position = mb1->rigidBody.position - mb1->rigidBody.velocity * t2;
		mb2->rigidBody.position = mb2->rigidBody.position - mb2->rigidBody.velocity * t2;

		//Velocity - x
		vNorm1 = mb1->rigidBody.velocity.x;
		vNorm2 = mb2->rigidBody.velocity.x;

		mb1->rigidBody.velocity.x = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.x = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.x = mb1->rigidBody.velocity.x * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.x = mb2->rigidBody.velocity.x * mb2->rigidBody.elasticity;

		//Velocity - z
		vNorm1 = mb1->rigidBody.velocity.z;
		vNorm2 = mb2->rigidBody.velocity.z;

		mb1->rigidBody.velocity.z = ((mb1->rigidBody.mass - mb2->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm1) + ((2 * mb2->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm2);
		mb2->rigidBody.velocity.z = ((mb2->rigidBody.mass - mb1->rigidBody.mass) / (mb1->rigidBody.mass + mb2->rigidBody.mass) * vNorm2) + ((2 * mb1->rigidBody.mass / (mb1->rigidBody.mass + mb2->rigidBody.mass)) * vNorm1);

		mb1->rigidBody.velocity.z = mb1->rigidBody.velocity.z * mb1->rigidBody.elasticity;
		mb2->rigidBody.velocity.z = mb2->rigidBody.velocity.z * mb2->rigidBody.elasticity;
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
bool ReflectMovingSphereStaticAABB(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb1, ColliderComponent* c2, float dt)
{
	glm::vec3 normal;
	float t = DynamicSphereToStaticAABB(&mb1->rigidBody.prevPosition, &mb1->rigidBody.position, c1->NarrowPhase.radius, &mb1->rigidBody.velocity, &sb1->position, &c2->NarrowPhase.maxPoint, &c2->NarrowPhase.minPoint, dt, &normal);

	if (t == -1)
		return false;
	if (t > dt)
		return false;

	mb1->rigidBody.position = mb1->rigidBody.prevPosition + t * mb1->rigidBody.velocity;

	float velocityAlongNormal = glm::dot(mb1->rigidBody.velocity, normal);

	LOG_INFO(velocityAlongNormal);

	// Calculate reflected velocity
	if (glm::dot(mb1->rigidBody.velocity, normal) < 0)
	{
		mb1->rigidBody.velocity += fabs(velocityAlongNormal) * normal;
		mb1->rigidBody.velocity += fabs(velocityAlongNormal) * normal * mb1->rigidBody.elasticity;
	}
	else
	{
		mb1->rigidBody.velocity += fabs(velocityAlongNormal) * normal * mb1->rigidBody.elasticity;
	}


	// Calculate reflected velocity
	if (glm::dot(mb1->rigidBody.velocity, normal) < 0)
	{
		mb1->rigidBody.velocity += fabs(velocityAlongNormal) * normal;
		mb1->rigidBody.velocity += fabs(velocityAlongNormal) * normal * mb1->rigidBody.elasticity;
	}
	else
	{
		mb1->rigidBody.velocity += fabs(velocityAlongNormal) * normal * mb1->rigidBody.elasticity;
	}
	
	mb1->rigidBody.position = mb1->rigidBody.position + (dt - t) * mb1->rigidBody.velocity;

	return true;
}


// For Sphere AABB interactions:
// Returns the squared distance between point c and segment ab
float SqDistPointSegment(glm::vec3* a, glm::vec3* b, glm::vec3* c)
{
	glm::vec3 ab = *a - *b;
	glm::vec3 ac = *c - *a;
	glm::vec3 bc = *c - *b;

	float e = glm::dot(ac, ab);
	// Handle cases where c projects outside ab
	if (e <= 0.0f) return glm::dot(ac, ac);
	float f = glm::dot(ab, ab);
	if (e >= f) return glm::dot(bc, bc);
	// Handle cases where c projects onto ab
	return glm::dot(ac, ac) - (e * e / f);
}

void ClosestPtPointSegment(glm::vec3* c, glm::vec3* a, glm::vec3* b, float& t, glm::vec3& d)
{
	glm::vec3 ab = *b - *a;
	// Project c onto ab, computing parameterized position d(t) = a + t*(b � a)
	t = glm::dot(*c - *a, ab) / glm::dot(ab, ab);
	// If outside segment, clamp t (and therefore d) to the closest endpoint
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;
	// Compute projected position from the clamped t
	d = *a + t * ab;
}