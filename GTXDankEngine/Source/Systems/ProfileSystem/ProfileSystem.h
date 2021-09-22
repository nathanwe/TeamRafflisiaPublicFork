#pragma once
#include "../../Core/System.h"
#include "../utils/Timer.h"

class ProfileSystem : public System
{

public:
	// default constructor
	ProfileSystem() = default;

	// delete copy and assignment constructor
	ProfileSystem(const ProfileSystem&) = delete;
	ProfileSystem& operator= (const ProfileSystem&) = delete;

	bool Init();

	void Update(float timeStamp);

	bool Destroy();

	struct ProfileResult 
	{
		const char* name;
		float time;
	};
	
	std::vector<ProfileResult> profileResults;

	void AddResult(const char* resultName, float resultTime);
	

private:

	GLFWwindow* openGLwindow;

};





