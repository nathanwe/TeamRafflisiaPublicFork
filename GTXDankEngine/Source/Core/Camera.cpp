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
}

void Camera::Init()
{
	engine.CommandSys.MoveCommand.SetActionToExecute([&](auto dir)
		{
			if (dir == MoveDirection::UP)
			{
				Position += speed * Orientation;
			}
			if (dir == MoveDirection::LEFT)
			{
				Position += speed * -glm::normalize(glm::cross(Orientation, Up));
			}
			if (dir == MoveDirection::DOWN)
			{
				Position += speed * -Orientation;
			}
			if (dir == MoveDirection::RIGHT)
			{
				Position += speed * glm::normalize(glm::cross(Orientation, Up));
			}
		});

	engine.CommandSys.SpaceCommand.SetActionToExecute([&]()
		{
			Position += speed / 2.f * Up;
		});
	engine.CommandSys.CtrlCommand.SetActionToExecute([&]()
		{
			Position += speed / 2.f * -Up;
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

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) 
{
	glUniformMatrix4fv(shader.getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	//{
	//	Position += 10* speed * Orientation;
	//}
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	//{
	//	Position += 10 * speed * -glm::normalize(glm::cross(Orientation, Up));
	//}
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	//{
	//	Position += 10 * speed * -Orientation;
	//}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	//{
	//	Position += 10 * speed * glm::normalize(glm::cross(Orientation, Up));
	//}
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
	//{
	//	Position += speed * Up;
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
	//{
	//	Position += speed * -Up;
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
	//{
	//	speed = 0.01f;
	//}
	//else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) 
	//{
	//	speed = 0.001f;
	//}

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

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))) 
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		//moved to imgui
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		firstClick = true;
	}

	//LOG_INFO("camera position: {0}, {1}, {2}", Position.x, Position.y, Position.z);
}