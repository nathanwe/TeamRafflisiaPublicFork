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
