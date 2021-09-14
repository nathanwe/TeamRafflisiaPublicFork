#pragma once

#include <string>

// forward declearation
class Model;

class GameObject
{
public:
	GameObject();
	~GameObject();
private:
	Model *tempModelUntillItsAComponent;
};



