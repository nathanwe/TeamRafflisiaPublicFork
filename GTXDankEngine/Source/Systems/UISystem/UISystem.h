#pragma once
#include "pch.h"

#include "../../Core/System.h"

class UISystem : public System
{

public:
	// default constructor
	UISystem() = default;

	// delete copy and assignment constructor
	UISystem(const UISystem&) = delete;
	UISystem& operator= (const UISystem&) = delete;

	bool Init();

	void Update(float timeStamp);

	bool Destroy();

	void GrabWindow(GLFWwindow* window);

private:

	int frameRate;
	GLFWwindow* openGLwindow;

};