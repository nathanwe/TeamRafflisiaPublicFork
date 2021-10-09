#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include"pch.h"
#include "../../Core/System.h"


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


#endif // !AUDIOSYSTEM_H


