#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include"pch.h"
#include "../../Core/System.h"
#include "../Systems/ScriptSystem/ScriptSystem.h"


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

	void HandleEvent(Event event);

public:

	FMOD_RESULT result;
	FMOD::Studio::System* system;
	bool allMuted;
	FMOD::Studio::Bus* masterBus = NULL;
	FMOD::Studio::EventInstance* eventInstance = NULL;
};


#endif // !AUDIOSYSTEM_H


