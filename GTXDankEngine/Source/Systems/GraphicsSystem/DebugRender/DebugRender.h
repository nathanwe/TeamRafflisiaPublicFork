#ifndef DEBUG_RENDER_H
#define DEBUG_RENDER_H

#include "pch.h"
#include "../Core/Camera.h"

class Shader;
class ColliderComponent;
class TransformComponent;

class DebugRender
{
public:
	DebugRender() = default;

	// accept a camera pointer, init 2 shaders
	void Init(Camera* camera);

	// delete 2 shaders
	void Destroy();

	
	// Render all light sources
	// 1. Iterate all entities with lightComponent
	// 2. all light sources are renderer as sphere
	// 3. Color of sphere is: lightComponent->LightSource->Color
	void RenderLightSource(GLuint fbo);


	//Ddraw all objects(forward renderer)
	// 1. Using NormalVevShader
	// 2. Geometry shader transform all input vertices into normal vectors
	// 3. Color of normal vector is passed as an uniform
	void DrawNormalVec();


	// Enable wireframe mode
	// draw all objects(forward renderer)
	// 1. Using LightSourceShader
	// 2. color of wireframe obj is passed as an uniform
	// Disable wireframe mode
	void DrawWireFrameObj();


	void DrawCollider();

	bool EnableNormalVisual = false;

private:
	Shader* LightSourceShader;
	Shader* NormalVecShader;
	Camera* camera;


	void RenderDebugSphereCollider(Shader* shader, ColliderComponent* collierCom, TransformComponent* transCom);
	void RenderDebugPlaneCollider(Shader* shader, ColliderComponent* collierCom, TransformComponent* transCom);
};



#endif
