#ifndef PROFILESYSTEM_H
#define PROFILESYSTEM_H

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
		double time;
	};
	
	std::vector<ProfileResult> profileResults;

	void AddResult(const char* resultName, double resultTime);
	

private:

	GLFWwindow* openGLwindow;

};


#endif


