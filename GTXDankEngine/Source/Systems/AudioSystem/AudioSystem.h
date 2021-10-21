#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include"pch.h"
#include "../Core/System.h"
#include "../Core/Camera.h"
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
	void HandleEvent(Event ev);

	void LoadBank(const char*, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const char* event_filename);
	void LoadSound(const char* strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	void UnLoadSound(const char* strSoundName);
	void Set3dListenerAndOrientation(Camera camera);
	void Set3dListenerAndOrientation(const glm::vec3& vPos = glm::vec3(0));
	int PlaySound(const char* strSoundName, const glm::vec3& vPos = glm::vec3(0), float fVolumedB = 0.0f);
	void PlayEvent(const char* event_filename);
	void StopChannel(int nChannelId);
	void StopEvent(const char* strEventName, bool bImmediate = false);
	void GetEventParameter(const char* strEventName, const char* strEventParameter, float* parameter);
	void SetEventParameter(const char* strEventName, const char* strParameterName, float fValue);
	void StopAllChannels();
	void SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	bool IsPlaying(int nChannelId) const;
	bool IsEventPlaying(const char* strEventName) const;
	float dBtoVolume(float db);
	float VolumeTOdB(float volume);
	//FMOD_VECTOR VectorToFmod(const Vector& vPosition);
	FMOD_VECTOR vec3GLMtoFMOD(const glm::vec3& vec3);
	void HandleEvent(Event event);
	void MuteAll();

public:

	FMOD_RESULT result;
		
	FMOD::System* coreSystem;
	FMOD::Studio::System* fmodStudioSystem;

	int nextChannelId;

	typedef std::map<const char*, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;
	typedef std::map<const char*, FMOD::Studio::EventInstance*> EventMap;
	typedef std::map<const char*, FMOD::Studio::Bank*> BankMap;
	
	BankMap bankMaps;
	EventMap eventMaps;
	SoundMap soundMaps;
	ChannelMap channelMaps;

	bool allMuted;
	FMOD::Studio::Bus* masterBus = NULL;
};


#endif // !AUDIOSYSTEM_H


