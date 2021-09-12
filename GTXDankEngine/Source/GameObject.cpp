#include "GameObject.h"

GameObject::GameObject()
{
	tempModelUntillItsAComponent = new Model("Assets/models/scroll/scene.gltf");
}

GameObject::~GameObject()
{
}
