#include "pch.h"

#include "DebugRender.h"
#include "../Core/Shader.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
//
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
//



void DebugRender::RenderLightSource(GLuint fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	LightSourceShader->setMat4("view", camera->GetViewMat());
	LightSourceShader->setMat4("projection", camera->GetProjMat(45.0f, 0.1f, 300.0f));

	std::set<Entity> LTMEntitys = LightComponentPool.Get3SharedEntitys(TransformComponentPool.componentList, ModelComponentPool.componentList);
	for (auto e : LTMEntitys)
	{
		auto lightComponent = LightComponentPool.GetComponentByEntity(e);
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
		auto modelComponent = ModelComponentPool.GetComponentByEntity(e);

		LightSourceShader->setVec3("lightColor", lightComponent->LightSource.Color);

		// light bulb is too small, make it larger
		//float oldScale = transformComponent->transform.scale;

		transformComponent->transform.scale = glm::vec3(5);
		LightSourceShader->setMat4("model", transformComponent->transform.Matrix());

		ResourceHandle<Model>* model = ModelResourceManager.GetResourceHandle("Assets/Models/LightBulb.obj");
		model->GetPointer()->Draw(*LightSourceShader);

		//transformComponent->transform.scale = oldScale;
		//LightSourceShader->setMat4("model", transformComponent->transform.Matrix());
		//modelComponent->model->GetPointer()->Draw(*LightSourceShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




void DebugRender::DrawNormalVec()
{
	NormalVecShader->setMat4("view", camera->GetViewMat());
	NormalVecShader->setMat4("projection", camera->GetProjMat(45.0f, 0.1f, 300.0f));

	NormalVecShader->setVec3("segmentColor", glm::vec3(1.0, 0.0, 0.0));

	std::set<Entity> entitys = MaterialComponentPool.Get3SharedEntitys(TransformComponentPool.componentList, ModelComponentPool.componentList);
	for (auto e : entitys)
	{
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
		auto modelComponent = ModelComponentPool.GetComponentByEntity(e);

		NormalVecShader->setMat4("model", transformComponent->transform.Matrix());
		modelComponent->model->GetPointer()->Draw(*NormalVecShader);
	}
}




void DebugRender::DrawWireFrameObj()
{
	LightSourceShader->setMat4("view", camera->GetViewMat());
	LightSourceShader->setMat4("projection", camera->GetProjMat(45.0f, 0.1f, 300.0f));

	// color of wireframe
	LightSourceShader->setVec3("lightColor", glm::vec3(0.0));
	// enable wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	std::set<Entity> entitys = MaterialComponentPool.Get3SharedEntitys(TransformComponentPool.componentList, ModelComponentPool.componentList);
	for (auto e : entitys)
	{
		auto transformComponent = TransformComponentPool.GetComponentByEntity(e);
		auto modelComponent = ModelComponentPool.GetComponentByEntity(e);

		LightSourceShader->setMat4("model", transformComponent->transform.Matrix());
		modelComponent->model->GetPointer()->Draw(*LightSourceShader);
	}
	// disable wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void DebugRender::Init(Camera* c)
{
	camera = c;

	LightSourceShader = new Shader("Source/Shaders/LightSource.shader");
	NormalVecShader = new Shader("Source/Shaders/DebugRender/NormalVec.shader");
}


void DebugRender::Destroy()
{
	delete LightSourceShader;
	delete NormalVecShader;
}