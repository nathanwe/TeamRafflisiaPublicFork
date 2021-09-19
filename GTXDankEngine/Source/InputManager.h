/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.h
Purpose: Handles game inputs
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void UpdateCamera(Camera camera, GLFWwindow* pWindow);
	bool IsKeyPressed(unsigned int KeyScanCode);
	bool IsKeyTriggered(unsigned int KeyScanCode);
	bool IsKeyReleased(unsigned int KeyScanCode);

	int mMouseX, mMouseY;
	bool IsMousePressed();
	bool IsMouseTriggered();
	bool IsMouseReleased();
public:



private:
private:
	unsigned int mCurrentState[512];
	unsigned int mPreviousState[512];

	bool mCurrentMouse;
	bool mPreviousMouse;

};

#endif