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
	//createStatic(45.f, 0.1f, 100.f, { 15,20,-20 }, 0, M_PI);

	/*
	std::vector<glm::vec3> p{ {0,0,0}, {5,5,5}, {30, 0, 5} };

	std::vector<float> a{ 0, -1.57 };
	std::vector<float> b{ 0, 0 };

	createCameraSpline(3.f, p, a,b);
	*/
}

void Camera::Update(float dt)
{
	autoScrollTimer += dt;
	railTimer += dt;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	if (isStatic) { return; }

	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::mat4(1.0f);

	// view = glm::lookAt(Position, Position + Orientation, Up);
	if (thirdPerson) {
		if (isOffset) {
			viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -thirdPersonOffset)) * glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), -Position - offset * orientationQuat);
		}
		else {
			viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -thirdPersonOffset)) * glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), -Position);
		}
	}
	else {
		if (isOffset) {
			viewMatrix = glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), -Position - offset * orientationQuat);
		}
		else {
			viewMatrix = glm::mat4_cast(orientationQuat) * glm::translate(glm::mat4(1.0f), -Position);
		}
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

	if (isStatic) { glfwSetCursorPos(window, (width / 2), (height / 2)); return; }

	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	//{
		//if (firstClick)
		//{
		//	glfwSetCursorPos(window, (width / 2), (height / 2));
		//	firstClick = false;
		//}

	updated = true;

	double mouseX = 0;
	double mouseY = 0;

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
	//}
	//else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		//firstClick = true;

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
	//}

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

		orientationScale = glm::vec3(sin(yaw) * cos(pitch), -sin(pitch), -cos(yaw) * cos(pitch));
	}

	speed = 10.0f;

	//LOG_INFO("camera position: {0}, {1}, {2}", Position.x, Position.y, Position.z);

	if (isOnRail) {
		if (railTimer >= railMaxTime) {
			railTimer = railMaxTime;
			isOnRail = false;
			isRailAngles = false;
		}

		float t = railTimer / railMaxTime * (railPoints.size() - 1);
		Position = railSpline(t);

		if (isRailAngles) {
			t = railTimer / railMaxTime * (railAngles.size() - 1);
			if (railAngles.size() <= 1 || t > railAngles.size() - 1) {
				orientationQuat = railAngles[railAngles.size() - 1];
				pitch = railPitch[railAngles.size() - 1];
				yaw = railYaw[railAngles.size() - 1];
			}
			else {
				orientationQuat = glm::mix(railAngles[int(t)], railAngles[int(t) + 1], t - int(t));
				pitch = glm::mix(railPitch[int(t)], railPitch[int(t) + 1], t - int(t));
				yaw = glm::mix(railYaw[int(t)], railYaw[int(t) + 1], t - int(t));
			}
		}

		return;
	}


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

void Camera::createCameraSpline(float time, std::vector<glm::vec3> points, std::vector<float> yaw = std::vector<float>(), std::vector<float> pitch = std::vector<float>()) {
	isOnRail = true;
	railPoints.clear();
	railPoints = points;

	railMaxTime = time;
	railTimer = 0.f;
	/*
	railTimes.clear();
	railTimes = time;
	railMaxTime = 0.f;
	for (auto t : railTimes) {
		railMaxTime += t;
	}
	*/

	if (pitch.size() > 0 && pitch.size() == yaw.size()) {
		isRailAngles = true;

		railPitch.clear();
		railPitch = pitch;

		railYaw.clear();
		railYaw = yaw;

		railAngles.clear();
		for (int i = 0; i < pitch.size(); i++) {
			railAngles.push_back(glm::angleAxis(pitch[i], glm::vec3(1, 0, 0)) * glm::angleAxis(yaw[i], glm::vec3(0, 1, 0)));
		}
	}
}

glm::vec3 Camera::railSpline(float t) {
	int p0, p1, p2, p3;

	p1 = (int)t % railPoints.size();
	p2 = (p1 + 1) % railPoints.size();
	p3 = (p2 + 1) % railPoints.size();
	p0 = p1 >= 1 ? (p1 - 1) % railPoints.size() : railPoints.size() - 1;

	t = t - (int)t;

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.0f * tt - t;
	float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
	float q3 = -3.0f * ttt + 4.0f * tt + t;
	float q4 = ttt - tt;

	float tx = 0.5f * (railPoints[p0].x * q1 + railPoints[p1].x * q2 + railPoints[p2].x * q3 + railPoints[p3].x * q4);
	float ty = 0.5f * (railPoints[p0].y * q1 + railPoints[p1].y * q2 + railPoints[p2].y * q3 + railPoints[p3].y * q4);
	float tz = 0.5f * (railPoints[p0].z * q1 + railPoints[p1].z * q2 + railPoints[p2].z * q3 + railPoints[p3].z * q4);

	return{ tx, ty, tz };
}

void Camera::createStatic(float FOVdeg, float nearPlane, float farPlane, glm::vec3 _position, float _pitch, float _yaw, float _roll) {
	pitch = _pitch;
	yaw = _yaw;
	roll = _roll;

	Position = _position;

	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));

	orientationQuat = qRoll * qPitch * qYaw;
	orientationQuat = glm::normalize(orientationQuat);

	orientationScale = glm::vec3(sin(yaw) * cos(pitch), -sin(pitch), -cos(yaw) * cos(pitch));

	isStatic = false;
	UpdateMatrix(FOVdeg, nearPlane, farPlane);
	isStatic = true;
}
