#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include"pch.h"
#include "../Core/System.h"
#include "../Core/Camera.h"
#include "../Systems/ScriptSystem/ScriptSystem.h"

struct WaitingSound
{
	const char* strSoundName;
	glm::vec3 vPos = glm::vec3(0);
	float fVolumedB = 0.0f;
};

struct ConstCharStarComparator
{
	//Custom Comparator for char*
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

typedef int EventID;

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

	void LoadBank(const char*, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void UnloadBank(const char*);
	void LoadBus(const char*);
	void UnloadBus(const char*);
	void LoadEvent(const char* event_filename);
	EventID PlayEvent(const char* event_filename);
	//void PlayEvent(EventID);
	void StopEvent(const char* strEventName, bool bImmediate = false);
	void StopEvent(EventID, bool bImmediate = false);
	//void PauseEvent(const char* strEventName, bool bImmediate = false);
	//void PauseEvent(EventID, bool bImmediate = false);
	float GetEventInstanceParameter(EventID id, const char* parameter_name);
	void SetEventInstanceParameter(EventID id, const char* parameter_name, float value);
	bool IsEventPlaying(EventID) const;

	//void LoadSound(const char* strSoundName, bool b3d = true, bool bLooping = false, bool bus = 0, bool bStream = false);
	//void UnLoadSound(const char* strSoundName);

	void SetBusMuted(FMOD::Studio::Bus*, bool = true);
	void SetBusMuted(const char*, bool = true);
	void SetBusVolume(FMOD::Studio::Bus*, float fVolumedB);
	void SetBusVolume(const char*, float fVolumedB);
	void MuteAll();

	void Set3dListenerAndOrientation(Camera camera);
	void Set3DAudioEventPos(EventID id, glm::vec3 vPos = glm::vec3(0));
	//void Set3dListenerAndOrientation(const glm::vec3& vPos = glm::vec3(0));
	//int fmodPlaySound(const char* strSoundName, const glm::vec3& vPos = glm::vec3(0), float fVolumedB = 0.0f);
	//void StopChannel(int nChannelId);
	

	//void GetEventParameter(const char* strEventName, const char* strEventParameter, float* parameter);
	//void SetEventParameter(const char* strEventName, const char* strParameterName, float fValue);
	//void StopAllChannels();
	//void SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
	//void SetChannelVolume(int nChannelId, float fVolumedB);
	
	//bool IsPlaying(int nChannelId) const;
	

	//Convertering functions
	float dBtoVolume(float db);
	float VolumeTOdB(float volume);
	FMOD_VECTOR vec3GLMtoFMOD(const glm::vec3& vec3);

	void HandleEvent(Event event);
	
	void TryPlayWaitingList();

	const char* FindWord(const char* word);
	
public:

	FMOD_RESULT result;

	FMOD::System* coreSystem;
	FMOD::Studio::System* fmodStudioSystem;

	FMOD::Studio::Bus* MasterBus;
	FMOD::Studio::Bus* BGMbus;
	FMOD::Studio::Bus* SFXbus;

	int nextChannelId;

	EventID currentEventID = 0;

	typedef std::map<const char*, FMOD::Sound*,ConstCharStarComparator> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;
	typedef std::map<const char*, FMOD::Studio::Bus*, ConstCharStarComparator> BusMap;
	typedef std::map<const char*, FMOD::Studio::EventDescription*, ConstCharStarComparator> EventMap;
	typedef std::map<EventID, std::pair< const char*, FMOD::Studio::EventInstance*>*> EventInstanceMap;
	typedef std::map<const char*, FMOD::Studio::Bank*, ConstCharStarComparator> BankMap;

	std::deque <std::string> wordDeque;

	BankMap bankMaps;
	EventMap eventMaps;
	EventInstanceMap  eventInstanceMaps;
	SoundMap soundMaps;
	ChannelMap channelMaps;
	BusMap busMaps;

	bool BGMMuted;
	bool SFXMuted;
	//scripts just directly touch these values between 1-10
	int BGMVolume = 5;
	int SFXVolume = 5;

	FMOD::Studio::Bus* masterBus = NULL;

	FMOD::ChannelGroup* BGM;
	FMOD::ChannelGroup* SFX;

	FMOD_3D_ATTRIBUTES attribute;

	std::list<WaitingSound> waitingList;
};


#endif // !AUDIOSYSTEM_H


