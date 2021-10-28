#include "pch.h"
#include "AudioSystem.h"
#include "../GraphicsSystem/GraphicsSystem.h"
#include "../Core/Engine.h"
#include "../utils/common.h"
#include <../glm/gtx/string_cast.hpp >

extern Engine engine;

bool AudioSystem::Init()
{
    fmodStudioSystem = NULL;
    ERRCHECK(FMOD::Studio::System::create(&fmodStudioSystem));

    // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
    coreSystem = NULL;
    ERRCHECK(fmodStudioSystem->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(fmodStudioSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL| FMOD_INIT_3D_RIGHTHANDED, 0));

    ERRCHECK(coreSystem->createChannelGroup("BGM", &BGM));
    ERRCHECK(coreSystem->createChannelGroup("SFX", &SFX));

    LoadBank("Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    LoadBank("Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    LoadEvent("event:/BGM");
    PlayEvent("event:/BGM");
    LoadSound("SaberRelay.mp3", true, true);
    int channel = PlaySound("SaberRelay.mp3");
    SetChannelVolume(channel,-3.0f);
    SetChannel3dPosition(channel, glm::vec3(-10.0f, -10.0f, 0.0f));

    ERRCHECK(fmodStudioSystem->getBus("bus:/", &masterBus));

    masterBus->setVolume(0.05f);

   /* eventInstance->start();
    eventInstance->setVolume(0.1f);*/
    Set3dListenerAndOrientation(engine.GraphicsSys.camera);
	return true;
}
void AudioSystem::Update(float timeStamp)
{
    Set3dListenerAndOrientation(engine.GraphicsSys.camera);

    std::vector<ChannelMap::iterator> pStoppedChannels;
    for (auto it = channelMaps.begin(), itEnd = channelMaps.end(); it != itEnd; ++it)
    {
        bool bIsPlaying = false;
        it->second->isPlaying(&bIsPlaying);
        if (!bIsPlaying)
        {
            pStoppedChannels.push_back(it);
        }
    }
    for (auto& it : pStoppedChannels)
    {
        channelMaps.erase(it);
    }

    MuteAll();
    ERRCHECK(fmodStudioSystem->update());
    
}
bool AudioSystem::Destroy()
{
    result = fmodStudioSystem->release();
    ERRCHECK(result);
    return !result;
}


void AudioSystem::LoadBank(const char* bank_filename, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
    auto mapIter = bankMaps.find(bank_filename);
    if (mapIter != bankMaps.end()) { return; }
    FMOD::Studio::Bank* pBank;
    ERRCHECK(fmodStudioSystem->loadBankFile(Common_MediaPath(bank_filename), flags, &pBank));
    if (pBank) { bankMaps[bank_filename] = pBank; }
}
void AudioSystem::LoadEvent(const char* event_filename)
{
    auto mapIter = eventMaps.find(event_filename);
    if (mapIter != eventMaps.end()) { return; }
    FMOD::Studio::EventDescription* pEventDescription = NULL;
    ERRCHECK(fmodStudioSystem->getEvent(event_filename, &pEventDescription));
    if (pEventDescription) {
        FMOD::Studio::EventInstance* pEventInstance = NULL;
        ERRCHECK(pEventDescription->createInstance(&pEventInstance));
        if (pEventInstance) {
            eventMaps[event_filename] = pEventInstance;
        }
    }
}
void AudioSystem::LoadSound(const char* sound_filename, bool b3d, bool bLooping, bool bStream, bool BUS)
{
    auto mapIter = soundMaps.find(sound_filename);
    if (mapIter != soundMaps.end())
        return;

    FMOD_MODE eMode = FMOD_DEFAULT;
    eMode |= b3d ? FMOD_3D : FMOD_2D;
    eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

    FMOD::Sound* pSound = nullptr;
    ERRCHECK(coreSystem->createSound(Common_MediaPath(sound_filename), eMode, nullptr, &pSound));
    if (pSound) {
        soundMaps[sound_filename] = pSound;
        groupMaps[sound_filename] = (int)BUS;
    }
}
void AudioSystem::UnLoadSound(const char* sound_filename)
{
    auto mapIter = soundMaps.find(sound_filename);
    if (mapIter == soundMaps.end()) { return; }
    ERRCHECK(mapIter->second->release());
    soundMaps.erase(mapIter);
}
int AudioSystem::PlaySound(const char* sound_filename, const glm::vec3& vPos, float fVolumedB)
{
    int nChannelId = nextChannelId++;
    auto mapIter = soundMaps.find(sound_filename);
    if (mapIter == soundMaps.end())
    {
        LoadSound(sound_filename);
        mapIter = soundMaps.find(sound_filename);
        if (mapIter == soundMaps.end())
        {
            return nChannelId;
        }
    }
    FMOD::Channel* pChannel = nullptr;
    ERRCHECK(coreSystem->playSound(mapIter->second, nullptr, true, &pChannel));
    if (pChannel)
    {
        FMOD_MODE currMode;
        mapIter->second->getMode(&currMode);
        if (currMode & FMOD_3D) {
            FMOD_VECTOR position = vec3GLMtoFMOD(vPos);
            ERRCHECK(pChannel->set3DAttributes(&position, nullptr));
        }
        ERRCHECK(pChannel->setVolume(dBtoVolume(fVolumedB)));
        ERRCHECK(pChannel->setPaused(false));
        channelMaps[nChannelId] = pChannel;
        ERRCHECK(pChannel->setChannelGroup(groupMaps[sound_filename] ? BGM : SFX));
    }
    return nChannelId;
}
void AudioSystem::Set3dListenerAndOrientation(Camera camera)
{
    FMOD_VECTOR pos = vec3GLMtoFMOD(camera.GetPosition());
    FMOD_VECTOR forward = vec3GLMtoFMOD(camera.Orientation);
    FMOD_VECTOR up = vec3GLMtoFMOD(camera.Up);

    //std::cout << glm::to_string(camera.Position) << glm::to_string(camera.Orientation) << glm::to_string(camera.Up) << std::endl;
    ERRCHECK(coreSystem->set3DListenerAttributes(0, &pos, nullptr, &forward, &up));
    //ERRCHECK(coreSystem->set3DListenerAttributes(0, &pos, nullptr, nullptr, nullptr));
}

void AudioSystem::PlayEvent(const char* event_filename)
{
    auto mapIter = eventMaps.find(event_filename);
    if (mapIter == eventMaps.end()) {
        LoadEvent(event_filename);
        mapIter = eventMaps.find(event_filename);
        if (mapIter == eventMaps.end()) { return; }
    }
    mapIter->second->start();
}
void AudioSystem::StopEvent(const char* event_filename, bool bImmediate) {
    auto mapIter = eventMaps.find(event_filename);
    if (mapIter == eventMaps.end()) { return; }

    FMOD_STUDIO_STOP_MODE eMode;
    eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
    ERRCHECK(mapIter->second->stop(eMode));
}

bool AudioSystem::IsEventPlaying(const char* event_filename) const {
    auto mapIter = eventMaps.find(event_filename);
    if (mapIter == eventMaps.end()) { return false; }


    FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
    if (mapIter->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
        return true;
    }
    return false;
}

void AudioSystem::SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition)
{

    auto mapIter = channelMaps.find(nChannelId);
    if (mapIter == channelMaps.end()) { return; }

    FMOD_VECTOR position = vec3GLMtoFMOD(vPosition);
    ERRCHECK(mapIter->second->set3DAttributes(&position, NULL));
}

void AudioSystem::SetChannelVolume(int nChannelId, float fVolumedB)
{
    auto mapIter = channelMaps.find(nChannelId);
    if (mapIter == channelMaps.end()) { return; }

    ERRCHECK(mapIter->second->setVolume(dBtoVolume(fVolumedB)));
}

void AudioSystem::GetEventParameter(const char* event_filename, const char* parameter_name, float* value) {
    auto mapIter = eventMaps.find(event_filename);
    if (mapIter == eventMaps.end()) { return; }
    ERRCHECK(mapIter->second->getParameterByName(parameter_name, value));
    //    FMOD::Studio::ParameterInstance* pParameter = NULL;
    //    CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
    //    CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
}

void AudioSystem::SetEventParameter(const char* event_filename, const char* parameter_name, float value) {
    auto mapIter = eventMaps.find(event_filename);
    if (mapIter == eventMaps.end()) { return; }
    ERRCHECK(mapIter->second->setParameterByName(parameter_name, value));
//    FMOD::Studio::ParameterInstance* pParameter = NULL;
//    CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
//    CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
}


FMOD_VECTOR AudioSystem::vec3GLMtoFMOD(const glm::vec3& vec3)
{
    FMOD_VECTOR fmodvec;
    fmodvec.x = vec3.x;
    fmodvec.y = vec3.y;
    fmodvec.z = vec3.z;

    return fmodvec;
}

float AudioSystem::dBtoVolume(float dB)
{
    return powf(10.0f, 0.05f * dB);
}

float AudioSystem::VolumeTOdB(float volume)
{
    return 20.0f * log10f(volume);
}

void AudioSystem::MuteAll()
{
    masterBus->setVolume(0.05f * !allMuted);
}

void AudioSystem::HandleEvent(Event event)
{
    if (event.type == EventType::MUTE_ALL)
    {
        allMuted = true;
    }
    if (event.type == EventType::UNMUTE_ALL)
    {
        allMuted = false;
    }
    if (event.type == EventType::PLAY_SOUND)
    {
        PlaySound(event.stringData1.c_str(), glm::vec3(event.floatData2, event.floatData3, event.floatData4), event.floatData1);
    }
}




