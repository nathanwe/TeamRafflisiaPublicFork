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
}

void Camera::Init()
{

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

	if (objectTrack) {
		Entity e = UINT32_MAX;
		for (auto comp : GameLogicCategoryComponentPool.componentList)
		{
			auto gLCComponent = comp.second;
			if (gLCComponent != nullptr)
			{
				if (gLCComponent->categories.find(GameLogicCategories::POKEBALL) != gLCComponent->categories.end())
				{
					e = comp.first;
					break;
				}
			}
		}

		if (e != UINT32_MAX) {
			updated = true;
			TransformComponent* pTrans = TransformComponentPool.GetComponentByEntity(e);
			Position = pTrans->transform->position;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		//moved to imgui
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		updated = true;

		double mouseX;
		double mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = 180 * sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = 180 * sensitivity * (float)(mouseX - (width / 2)) / width;

		Orientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		pitch += sensitivity * (float)(mouseY - (height / 2)) / height;
		yaw += sensitivity * (float)(mouseX - (width / 2)) / width;

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		//moved to imgui
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		firstClick = true;

		if (engine.InputSys.IsControllerActive(0)) {
			updated = true;

			float rotX = 0.f;
			float rotY = 0.f;

			if (abs(engine.InputSys.GetControllerAxis(0, 2)) >= 0.1f) {
				rotX = gamepadSensitivity * engine.InputSys.GetControllerAxis(0, 2);
			}
			if (abs(engine.InputSys.GetControllerAxis(0, 3)) >= 0.1f) {
				rotY = gamepadSensitivity * engine.InputSys.GetControllerAxis(0, 3);
			}

			yaw += rotX;
			pitch += rotY;
		}
	}

	if (updated)
	{
		if (pitch > M_PI / 2.f) {
			pitch = M_PI / 2.f;
		}
		if (pitch < -M_PI / 2.f) {
			pitch = -M_PI / 2.f;
		}

		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));

		orientationQuat = qRoll * qPitch * qYaw;
		orientationQuat = glm::normalize(orientationQuat);
	}

	//LOG_INFO("camera position: {0}, {1}, {2}", Position.x, Position.y, Position.z);
}