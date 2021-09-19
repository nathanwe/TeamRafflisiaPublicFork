#pragma once
#include "pch.h"

#include "Shader.h"

class Camera {
	public:
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		bool firstClick = true;

		int width;
		int height;

		float speed = 100.0f;
		float sensitivity = 100.0f;

		Camera(int width, int height, glm::vec3 position);

		void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
		void Matrix(Shader& shader, const char* uniform);
		void Inputs(GLFWwindow* window);
};

