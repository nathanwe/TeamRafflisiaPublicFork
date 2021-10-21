/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.cpp
Purpose: Handles game inputs
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/
#include "pch.h"
#include "InputManager.h"



bool InputManager::Init(GLFWwindow* pWindow) {
	memset(mCurrentState, 0, 512 * sizeof(bool));
	memset(mPreviousState, 0, 512 * sizeof(bool));
	memset(mCurrentStateThread, 0, 512 * sizeof(bool));

	mouseX = mouseY = mouseXPrev = mouseYPrev = 0.0f;

	updateThread = std::thread(&InputManager::UpdateThread, this, pWindow);
	return true;
}

InputManager::~InputManager() {
	threadRunning = false;
	std::unique_lock<std::mutex> inputUpdateLock(mtx);
	cv.notify_one();
	updateThread.join();
}

void InputManager::Update() 
{

	mouseXPrev = mouseX;
	mouseYPrev = mouseY;
	mouseX = mouseXThread;
	mouseY = mouseYThread;

	mPreviousMouse = mCurrentMouse;
	mCurrentMouse = mMouseThread;

	memcpy(mPreviousState, mCurrentState, input_buffer_size * sizeof(bool));
	std::swap(mCurrentState, mCurrentStateThread);

	for (int i = 0; i < gamepad_count; i++) {
		gamepads_prev[i] = gamepads[i];
		gamepads[i] = gamepads_thread[i];
	}

	std::unique_lock<std::mutex> inputUpdateLock(mtx);
	cv.notify_one();

}




void InputManager::UpdateThread(GLFWwindow* pWindow)
{
	// glfwSetKeyCallback(pWindow, KeyCallback);

	while (threadRunning) {

		for (int i = 0; i < input_buffer_size; i++) {
			if (glfwGetKey(pWindow, i) == GLFW_PRESS) {
				mCurrentStateThread[i] = 1;
			}
			else if (glfwGetKey(pWindow, i) == GLFW_RELEASE) {
				mCurrentStateThread[i] = 0;
			}
		}

		glfwGetCursorPos(pWindow, &mouseXThread, &mouseYThread);

		mMouseThread.left = (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
		mMouseThread.middle = (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
		mMouseThread.right = (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

		// Gamepads
		for (int scanned_controller = 0; scanned_controller < gamepad_count; scanned_controller++) {
			if (glfwJoystickPresent(GLFW_JOYSTICK_1 + scanned_controller) == GL_TRUE)
			{
				int axesCount, buttonCount;
				const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
				const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

				// Axis hierarchy (xbox layout, 6 axes)
				// L_JOY_X L_JOY_Y
				// R_JOY_X R_JOY_Y
				// LT RT

				for (int i = 0; i < 6; i++) {	// we are only accounting for 6 axes, regardless of controller
					if (axesCount <= i) {		// if a controller has fewer than 6 axes, its excess axes are treated as zero
						gamepads_thread[scanned_controller].axes[i] = 0.0f;
					}
					else {
						gamepads_thread[scanned_controller].axes[i] = axes[i];
					}
				}


				// Button hierarchy (xbox layout, 14 buttons):
				// A B X Y 
				// LB RB Select Start
				// L_JOY R_JOY
				// D-Pad: Up Right Down Left

				for (int i = 0; i < 14; i++) {	// we are only accounting for 14 buttons, regardless of controller
					if (axesCount <= i) {		// if a controller has fewer than 14 buttons, its excess are treated as unpressed
						gamepads_thread[scanned_controller].buttons[i] = GLFW_RELEASE;
					}
					else {
						gamepads_thread[scanned_controller].buttons[i] = (buttons[i] == GLFW_PRESS);
					}
				}

			}
			else 
			{	// if a controller is not connected to this port, it is zeroed out
				for (int i = 0; i < 6; i++) {
					gamepads_thread[scanned_controller].axes[i] = 0.0f;
				}
				for (int i = 0; i < 14; i++) {
					gamepads_thread[scanned_controller].buttons[i] = GLFW_RELEASE;
				}
			}
		}

		std::unique_lock<std::mutex> inputUpdateLock(mtx);
		cv.wait(inputUpdateLock);

	}
}

void InputManager::UpdateCamera(Camera camera, GLFWwindow* pWindow, InputManager* im) {
	camera.Inputs(pWindow);
}

bool InputManager::IsKeyPressed(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyTriggered(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode] && !mPreviousState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyReleased(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (mPreviousState[KeyScanCode] && !mCurrentState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsMousePressed() {
	return mCurrentMouse.left;
}

bool InputManager::IsMouseTriggered() {
	return (mCurrentMouse.left && !mPreviousMouse.left);
}

bool InputManager::IsMouseReleased() {
	return (!mCurrentMouse.left && mPreviousMouse.left);
}






float InputManager::GetControllerAxis(int controllerNo, int axis) {
	if (controllerNo < 0 || controllerNo >= gamepad_count || axis < 0 || axis >= 6) {
		return 0.0f;
	}
	return gamepads[controllerNo].axes[axis];
}

bool InputManager::IsControllerPressed(int controllerNo, int button) {
	if (controllerNo < 0 || controllerNo >= gamepad_count || button < 0 || button >= 14) {
		return false;
	}
	return gamepads[controllerNo].buttons[button];
}

bool InputManager::IsControllerTriggered(int controllerNo, int button) {
	if (controllerNo < 0 || controllerNo >= gamepad_count || button < 0 || button >= 14) {
		return false;
	}
	return (gamepads[controllerNo].buttons[button] && !gamepads_prev[controllerNo].buttons[button]);
}

bool InputManager::IsControllerReleased(int controllerNo, int button) {
	if (controllerNo < 0 || controllerNo >= gamepad_count || button < 0 || button >= 14) {
		return false;
	}
	return (!gamepads[controllerNo].buttons[button] && gamepads_prev[controllerNo].buttons[button]);
}
