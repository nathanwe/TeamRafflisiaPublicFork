#ifndef CAMERA_H
#define CAMERA_H

#define M_PI 3.141592653

#include "pch.h"

#include "Shader.h"

class Camera {
	public:
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		glm::quat orientationQuat = glm::normalize(glm::quat(1.f, 0.f, 0.f, 1.f));
		float pitch = glm::radians(0.0f);
		float yaw = glm::radians(180.0f);

		bool firstClick = true;

		int width;
		int height;

		float speed = 100.0f;
		float sensitivity = 1.0f;
		float gamepadSensitivity = 0.05f;

		Camera() {};
		Camera(int width, int height, glm::vec3 position);

		void Init();

		void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
		void Matrix(Shader& shader, const char* uniform);
		void Inputs(GLFWwindow* window);

		inline glm::mat4 GetViewMat() { return glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), Position); }
		inline glm::mat4 GetProjMat(float FOV, float n, float f) { return glm::perspective(glm::radians(FOV), (float)(width) / (height), n, f); }

	
};
#endif
