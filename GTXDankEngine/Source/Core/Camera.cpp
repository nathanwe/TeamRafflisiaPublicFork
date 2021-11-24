#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "../utils/Log.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"


extern Engine engine;

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;

	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(0, 0, 1));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));

	orientationQuat = qRoll * qPitch * qYaw;
	orientationQuat = glm::normalize(orientationQuat);

	orientationScale = glm::vec3(sin(yaw) * cos(pitch), -sin(pitch), -cos(yaw) * cos(pitch));
}

void Camera::Init()
{
	/*
	autoScroll = true;
	autoScrollMaxTime = 10.f;
	autoScrollTimer = 0.f;
	autoScrollBegin = glm::vec3(0, 5, -20);
	autoScrollDest = glm::vec3(-5, -5, 5);
	*/
}

void Camera::Update(float dt)
{
	autoScrollTimer += dt;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::mat4(1.0f);

	// view = glm::lookAt(Position, Position + Orientation, Up);
	if (thirdPerson) {
		viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -thirdPersonOffset)) * glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), -Position);
	}
	else {
		viewMatrix = glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), -Position);
	}
	projectionMatrix = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = projectionMatrix * viewMatrix;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(shader.getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	bool updated = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		updated = true;

		double mouseX;
		double mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseX - (width / 2)) / width;
		float rotY = sensitivity * (float)(mouseY - (height / 2)) / height;
		glfwSetCursorPos(window, (width / 2), (height / 2));

		if (engine.CommandSys.setting.mouseInvertedX) rotX *= -1.f;
		if (engine.CommandSys.setting.mouseInvertedY) rotY *= -1.f;

		pitch += rotY;
		yaw += rotX;

		// Support for sound system; TODO remove
		Orientation = glm::rotate(Orientation, glm::radians(-rotX * 180.f), glm::normalize(glm::cross(Orientation, Up)));
		Orientation = glm::rotate(Orientation, glm::radians(-rotY * 180.f), Up);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		firstClick = true;

		if (engine.InputSys.IsControllerActive(0)) {
			updated = true;

			float rotX = 0.f;
			float rotY = 0.f;

			if (abs(engine.InputSys.GetControllerAxis(0, 2)) >= 0.1f) {
				rotX = gamepadSensitivity * engine.InputSys.GetControllerAxis(0, 2);
				if (engine.CommandSys.setting.gamepadInvertedX) {
					rotX *= -1.f;
				}
			}
			if (abs(engine.InputSys.GetControllerAxis(0, 3)) >= 0.1f) {
				rotY = gamepadSensitivity * engine.InputSys.GetControllerAxis(0, 3);
				if (engine.CommandSys.setting.gamepadInvertedY) {
					rotY *= -1.f;
				}
			}

			// Support for sound system; TODO remove
			Orientation = glm::rotate(Orientation, glm::radians(180 * -rotX), glm::normalize(glm::cross(Orientation, Up)));
			Orientation = glm::rotate(Orientation, glm::radians(180 * -rotY), Up);

			yaw += rotX;
			pitch += rotY;
		}
	}

	if (updated)
	{
		if (pitch > M_PI / 2.f) {
			pitch = static_cast<float>(M_PI / 2.f);
		}
		if (pitch < -M_PI / 2.f) {
			pitch = static_cast<float>(-M_PI / 2.f);
		}

		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));

		orientationQuat = qRoll * qPitch * qYaw;
		orientationQuat = glm::normalize(orientationQuat);

		orientationScale = glm::vec3(sin(yaw) * cos(pitch), -sin(pitch), -cos(yaw) * cos(pitch));
	}

	speed = 10.0f;

	//LOG_INFO("camera position: {0}, {1}, {2}", Position.x, Position.y, Position.z);

	if (autoScroll) {
		if (autoScrollTimer >= autoScrollMaxTime) {
			autoScroll = false;
			Position = autoScrollDest;
			return;
		}

		Position = glm::mix(autoScrollBegin, autoScrollDest, autoScrollTimer / autoScrollMaxTime);
		return;
	}

	if (objectTrack) {
		Entity e = UINT32_MAX;
		for (auto comp : GameLogicCategoryComponentPool.componentList)
		{
			auto gLCComponent = comp.second;
			if (gLCComponent != nullptr)
			{
				if (gLCComponent->categories.find(GameLogicCategories::PLAYER) != gLCComponent->categories.end())
				{
					e = comp.first;
					break;
				}
			}
		}

		if (e != UINT32_MAX) {
			updated = true;
			TransformComponent* pTrans = TransformComponentPool.GetComponentByEntity(e);
			Position = pTrans->transform.position;
		}
	}

}

void Camera::SetAutoScroll(glm::vec3 begin, glm::vec3 dest, float time) {
	autoScrollMaxTime = time;
	autoScrollTimer = 0.f;
	autoScroll = true;

	autoScrollBegin = begin;
	autoScrollDest = dest;
}
