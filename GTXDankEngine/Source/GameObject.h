#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "Model.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
private:
	Model *tempModelUntillItsAComponent;
};

#endif

