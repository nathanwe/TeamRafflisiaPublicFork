#pragma once
#include "../../Core/System.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"

class AudioSystem : public System
{

public:
	// default constructor
	AudioSystem() = default;

	// delete copy and assignment constructor
	AudioSystem(const AudioSystem&) = delete;
	AudioSystem& operator= (const AudioSystem&) = delete;

	bool Init();

	void Update(float timeStamp);

	bool Destroy();

public:

	FMOD_RESULT result;
	FMOD::Studio::System* system;
};





