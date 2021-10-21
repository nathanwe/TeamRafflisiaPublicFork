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
	double* GetMouseChange();
	bool IsMousePressed();
	bool IsMouseTriggered();
	bool IsMouseReleased();

	bool IsControllerActive(int controllerNo) { return gamepads[controllerNo].active; }
	float GetControllerAxis(int controllerNo, int axis);
	bool IsControllerPressed(int controllerNo, int button);
	bool IsControllerTriggered(int controllerNo, int button);
	bool IsControllerReleased(int controllerNo, int button);
public:




private:
	// bool mkActive, controllerActive;
	// bool mkActiveThread, controllerActiveThread;

private:
	static const int input_buffer_size = 512;
	float deadzone = 0.1f;

	static const int gamepad_count = 8;
	struct Gamepad {
		float axes[6];
		bool buttons[14];
		bool active = true;
	};
	Gamepad gamepads_thread[gamepad_count];
	Gamepad gamepads[gamepad_count];
	Gamepad gamepads_prev[gamepad_count];


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
