#include "BlackBoxModelComponent.h"

BlackBoxModelComponent::BlackBoxModelComponent(std::shared_ptr<Model> _model) : model(_model)// Model("Assets/models/scroll/scene.gltf"))
{
}

BlackBoxModelComponent::~BlackBoxModelComponent()
{
	
}

void BlackBoxModelComponent::Update(std::chrono::milliseconds dt)
{
}

void BlackBoxModelComponent::HandleEvent(Event& event)
{
}

ComponentType BlackBoxModelComponent::GetType()
{
	return ComponentType::BLACK_BOX_MODEL;
}

void BlackBoxModelComponent::Draw(Shader shaderProgram, Camera camera)
{
	model->Draw(shaderProgram, camera);
}
