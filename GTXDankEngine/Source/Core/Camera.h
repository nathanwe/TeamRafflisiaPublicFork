#ifndef CAMERA_H
#define CAMERA_H

#define M_PI 3.141592653f

#include "pch.h"

#include "Shader.h"

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	glm::quat orientationQuat = glm::normalize(glm::quat(1.f, 0.f, 0.f, 1.f));
	glm::vec3 orientationScale;
	float pitch = glm::radians(0.0f);
	float yaw = glm::radians(0.0f);
	float roll = glm::radians(0.0f);

	bool firstClick = true;
	bool isStatic = false;

	int width;
	int height;

	float speed = 100.0f;
	float sensitivity = 1.0f;
	float gamepadSensitivity = 0.05f;

	bool mouseInvertX = false;
	bool mouseInvertY = false;
	bool gamepadInvertX = false;
	bool gamepadInvertY = false;

	bool objectTrack = false;
	bool thirdPerson = false;
	float thirdPersonOffset = 5.f;

	bool autoScroll = false;
	float autoScrollMaxTime, autoScrollTimer;
	glm::vec3 autoScrollBegin, autoScrollDest;

	bool isOffset = false;
	glm::vec3 offset = glm::vec3(0, 2, 0);


public:
	Camera(int width, int height, glm::vec3 position);

	void Init();
	void Update(float dt);

	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);

	void SetAutoScroll(glm::vec3 begin, glm::vec3 dest, float time);

public:
	inline glm::mat4 GetViewMat() { return viewMatrix; }
	inline glm::mat4 GetProjMat(float FOV, float n, float f) { return glm::perspective(glm::radians(FOV), (float)(width) / (height), n, f); }
	// inline glm::vec3 GetPosition() { return (Position - glm::vec3(thirdPersonOffset) * orientationScale); }
	inline glm::vec3 GetPosition() { return (Position + (isOffset ? -offset : glm::vec3(0.f)) - (thirdPerson ? glm::vec3(thirdPersonOffset) : glm::vec3(0.f)) * orientationScale); }
	//inline glm::vec3 GetRotation() { return glm::eulerAngles(orientationQuat) * 180.0f / 3.141f; }


public:
	bool isOnRail = false;
	bool isRailAngles = false;
	std::vector<glm::vec3> railPoints;

	std::vector<glm::quat> railAngles;
	std::vector<float> railPitch;
	std::vector<float> railYaw;

	std::vector<float> railTimes;
	float railTimer, railMaxTime;

	void createCameraSpline(float time, std::vector<glm::vec3> points, std::vector<float> yaw, std::vector<float> pitch);

	glm::vec3 railSpline(float t);

	void createStatic(float FOVdeg, float nearPlane, float farPlane, glm::vec3 _position = glm::vec3(0.f, 0.f, 0.f), float _pitch = 0.f, float _yaw = 0.f, float _roll = 0.f);


};
#endif
