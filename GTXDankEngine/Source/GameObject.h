#pragma once

#include <string>

#include "../Components/ModelComponent/Model.h"
#include <vector>
#include <chrono>

#include "AbstractComponent.h"
#include "BlackBoxModelComponent.h"
#include "Enums.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	Model *tempModelUntillItsAComponent;
	void Update(std::chrono::milliseconds dt);
	void HandleEvent(Event& event);
	AbstractComponent* getComponentPointer(ComponentType type);
	ErrorEnum AddComponent(AbstractComponent* componentToAdd);

private:
	std::vector<AbstractComponent*> components;
};



