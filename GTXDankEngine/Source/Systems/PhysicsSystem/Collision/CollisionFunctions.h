#pragma once
#ifndef COLLISION_FUNCTIONS_H
#define COLLISION_FUNCTIONS_H

#include "pch.h"

class MovingBodyComponent;
class StillBodyComponent;


int StaticPointToStaticSphere(glm::vec3* pP, glm::vec3* pCenter, float Radius);
int StaticSphereToStaticSphere(glm::vec3* pCenter0, float Radius0, glm::vec3* pCenter1, float Radius1);
int StaticSphereToStacticPlane(glm::vec3* pCenter, float Radius, glm::vec3* normal, float magnitude);

float DynamicSphereToStaticPlane(glm::vec3* pCenter0, glm::vec3* pCenter1, float radius, glm::vec3* velocity, glm::vec3* normal, float magnitude);

float DynamicPointToStaticSphere(glm::vec3* p1, glm::vec3* p2, glm::vec3* velocity, glm::vec3* pCenter, float Radius);
float DynamicSphereToStaticSphere(glm::vec3* pCenter0i, glm::vec3* pCenter0f, float Radius0, glm::vec3* velocity, glm::vec3* pCenter1, float Radius1);

float DynamicSphereToDynamicSphere(glm::vec3* pCenter0i, float Radius0, glm::vec3* velocity0, glm::vec3* pCenter1, float Radius1, glm::vec3* velocity1, float t);


//void ReflectSpherePlane(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
void ReflectStaticSphereStaticSphere(MovingBodyComponent* mb1, StillBodyComponent* sb2);
void ReflectMovingSphereMovingSphere(MovingBodyComponent* mb1, MovingBodyComponent* mb2, float dt);

#endif // !COLLISION_FUNCTIONS_H

