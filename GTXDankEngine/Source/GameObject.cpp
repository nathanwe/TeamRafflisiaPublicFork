#include "GameObject.h"

GameObject::GameObject()
{
	
}

GameObject::~GameObject()
{
}

void GameObject::Update(std::chrono::milliseconds dt)
{
	for (auto component : components)
	{
		component->Update(dt);
	}
}

void GameObject::HandleEvent(Event& event)
{
	for (auto component : components)
	{
		component->HandleEvent(event);
	}
}

AbstractComponent* GameObject::getComponentPointer(ComponentType type)
{
	for (auto component : components)
	{
		if (component->GetType() == type)
		{
			return component;
		}
		
	}
	return nullptr;
}

ErrorEnum GameObject::AddComponent(AbstractComponent* componentToAdd)
{
	ComponentType newType = componentToAdd->GetType();
	bool alreayHasOne = getComponentPointer(newType) != nullptr;
	if (alreayHasOne)
	{
		return ErrorEnum::FAIL_BECAUSE_COMPONENT_ALREADY_EXISTS;
	}
	components.push_back(componentToAdd);
	return ErrorEnum::SUCESS;
}
