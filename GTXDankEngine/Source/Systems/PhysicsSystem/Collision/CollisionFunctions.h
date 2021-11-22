#ifndef COLLISION_FUNCTIONS_H
#define COLLISION_FUNCTIONS_H

#include "pch.h"

class MovingBodyComponent;
class StillBodyComponent;
class ColliderComponent;


int StaticPointToStaticSphere(glm::vec3* pP, glm::vec3* pCenter, float Radius);
int StaticSphereToStaticSphere(glm::vec3* pCenter0, float Radius0, glm::vec3* pCenter1, float Radius1);
int StaticSphereToStacticPlane(glm::vec3* pCenter, float Radius, glm::vec3* normal, float magnitude);

int StaticAABBToStaticAABB(glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* p2, glm::vec3* max2, glm::vec3* min2);
int StaticSphereToStaticAABB(glm::vec3* pCenter0, float Radius0, glm::vec3* p1, glm::vec3* max1, glm::vec3* min1);
int StaticAABBToStaticPlane(glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, glm::vec3* normal, float magnitude);

float DynamicPointToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, glm::vec3* velocity, glm::vec3* normal, float magnitude);
float DynamicSphereToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* normal, float magnitude);

float DynamicPointToStaticSphere(glm::vec3* p1, glm::vec3* p2, glm::vec3* velocity, glm::vec3* pCenter, float Radius);
float DynamicSphereToStaticSphere(glm::vec3* pCenter0i, glm::vec3* pCenter0f, float Radius0, glm::vec3* velocity, glm::vec3* pCenter1, float Radius1);

float DynamicSphereToDynamicSphere(glm::vec3* pCenter0i, float Radius0, glm::vec3* velocity0, glm::vec3* pCenter1, float Radius1, glm::vec3* velocity1, float t);

float DynamicPointToStaticAABB(glm::vec3* pCenter0, glm::vec3* pCenter1, glm::vec3* velocity, glm::vec3* p1, glm::vec3* max1, glm::vec3* min1, float t);
//float DynamicSphereToStaticAABB(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* p, glm::vec3* max, glm::vec3* min, float t);


bool ReflectMovingSphereStaticPlane(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb2, ColliderComponent* c2, float dt);
bool ReflectMovingSphereStaticSphere(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb2, ColliderComponent* c2);
bool ReflectMovingSphereMovingSphere(MovingBodyComponent* mb1, ColliderComponent* c1, MovingBodyComponent* mb2, ColliderComponent* c2, float dt);

//bool ReflectMovingSphereStaticPlane(MovingBodyComponent* mb1, StillBodyComponent* sb2, float dt);
//
//bool ReflectMovingSphereStaticSphere(MovingBodyComponent* mb1, StillBodyComponent* sb2);
//
//bool ReflectMovingSphereMovingSphere(MovingBodyComponent* mb1, MovingBodyComponent* mb2, float dt);

bool ReflectMovingAABBStaticAABB(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb1, ColliderComponent* c2);
bool ReflectMovingAABBMovingAABB(MovingBodyComponent* mb1, ColliderComponent* c1, MovingBodyComponent* mb2, ColliderComponent* c2);

bool ReflectMovingAABBStaticPlane(MovingBodyComponent* mb1, ColliderComponent* c1, StillBodyComponent* sb1, ColliderComponent* c2);

#endif // !COLLISION_FUNCTIONS_H

