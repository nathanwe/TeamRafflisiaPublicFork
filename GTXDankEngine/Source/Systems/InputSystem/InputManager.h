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

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H



#include "pch.h"
#include "../Core/Camera.h"

class InputManager
{
public:
	InputManager() = default;
	~InputManager();

	bool Init(GLFWwindow* pWindow);

	void Update();
	void UpdateCamera(Camera camera, GLFWwindow* pWindow, InputManager* im);
	bool IsKeyPressed(unsigned int KeyScanCode);
	bool IsKeyTriggered(unsigned int KeyScanCode);
	bool IsKeyReleased(unsigned int KeyScanCode);

	double mouseX, mouseY;
	bool IsMousePressed();
	bool IsMouseTriggered();
	bool IsMouseReleased();
public:



private:
	static const int input_buffer_size = 512;

private:
	bool mCurrentState[input_buffer_size];
	bool mPreviousState[input_buffer_size];

	bool mCurrentStateThread[input_buffer_size];
	// unsigned int mPreviousStateThread[input_buffer_size];

	struct mouseButton {
		bool left = false;
		bool middle = false;
		bool right = false;
	};

	mouseButton mCurrentMouse, mPreviousMouse, mMouseThread;

	double mouseXThread, mouseYThread;
	double mouseXPrev, mouseYPrev;

	std::thread updateThread;
	std::mutex mtx;
	std::condition_variable cv;

	bool threadRunning = true;

	void UpdateThread(GLFWwindow* pWindow);

};
#endif // !INPUTMANAGER_H
