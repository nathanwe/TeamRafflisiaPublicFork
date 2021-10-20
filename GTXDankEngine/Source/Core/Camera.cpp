#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "../utils/Log.h"

extern Engine engine;

Camera::Camera(int width, int height, glm::vec3 position) 
{
	Camera::width = width;
	Camera::height = height;
	Position = position;

	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

	orientationQuat = qPitch * qYaw;
	orientationQuat = glm::normalize(orientationQuat);
}

void Camera::Init()
{
	engine.CommandSys.MoveCommand.SetActionToExecute([&](auto dir)
		{
			glm::vec3 tar(-sin(yaw) * cos(pitch), sin(pitch), cos(yaw) * cos(pitch));
			
			if (dir == MoveDirection::UP)
			{
				Position += speed * tar;
			}
			if (dir == MoveDirection::LEFT)
			{
				Position += speed * -glm::normalize(glm::cross(tar, Up));
			}
			if (dir == MoveDirection::DOWN)
			{
				Position += speed * -tar;
			}
			if (dir == MoveDirection::RIGHT)
			{
				Position += speed * glm::normalize(glm::cross(tar, Up));
			}
		});

	engine.CommandSys.SpaceCommand.SetActionToExecute([&]()
		{
			Position -= speed / 2.f * Up;
		});
	engine.CommandSys.CtrlCommand.SetActionToExecute([&]()
		{
			Position -= speed / 2.f * -Up;
		});
	engine.CommandSys.ShiftCommand.SetActionToExecute([&]()
		{
			speed = 0.5f;
		});
	engine.CommandSys.UnShiftCommand.SetActionToExecute([&]()
		{
			speed = 0.1f;
		});
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) 
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// view = glm::lookAt(Position, Position + Orientation, Up);
	view = glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), Position);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) 
{
	glUniformMatrix4fv(shader.getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
	{
		//moved to imgui
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) 
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		
		glfwGetCursorPos(window, &mouseX, &mouseY);

		pitch += sensitivity * (float)(mouseY - (height / 2)) / height;
		yaw += sensitivity * (float)(mouseX - (width / 2)) / width;

		if (pitch > M_PI / 2.f) {
			pitch = M_PI / 2.f;
		}
		if (pitch < -M_PI / 2.f) {
			pitch = -M_PI / 2.f;
		}


		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

		orientationQuat = qPitch * qYaw;
		orientationQuat = glm::normalize(orientationQuat);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		//moved to imgui
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		firstClick = true;

		if (engine.InputSys.IsControllerActive(0)) {
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

			if (pitch > M_PI / 2.f) {
				pitch = M_PI / 2.f;
			}
			if (pitch < -M_PI / 2.f) {
				pitch = -M_PI / 2.f;
			}

			glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
			glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

			orientationQuat = qPitch * qYaw;
			orientationQuat = glm::normalize(orientationQuat);
		}
	}

	//LOG_INFO("camera position: {0}, {1}, {2}", Position.x, Position.y, Position.z);
}