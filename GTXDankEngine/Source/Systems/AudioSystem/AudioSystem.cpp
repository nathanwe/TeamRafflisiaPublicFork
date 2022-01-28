#include "pch.h"
#include "AudioSystem.h"
#include "../GraphicsSystem/GraphicsSystem.h"
#include "../ProfileSystem/ProfileSystem.h"
#include "../Core/Engine.h"
#include "../utils/common.h"
#include <../glm/gtx/string_cast.hpp >

extern Engine engine;

template <typename T1, typename T2,typename T3>
T2 IsKeyInMap(std::map<T1, T2, T3>map, T1 key)
{
    auto mapIter = map.find(key);
    if (mapIter != map.end()) { return mapIter->second; }
    return nullptr;
}

bool AudioSystem::Init()
{
    PROFILE_THIS("Audio Init");

    fmodStudioSystem = NULL;
    ERRCHECK(FMOD::Studio::System::create(&fmodStudioSystem));

    // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
    coreSystem = NULL;
    ERRCHECK(fmodStudioSystem->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(fmodStudioSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL|FMOD_INIT_3D_RIGHTHANDED, 0));

    LoadBank("Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    LoadBank("Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    LoadBank("BackGround.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    LoadBank("SFXs.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);

    LoadBus("Bus:/");
    LoadBus("Bus:/BGMBus");
    LoadBus("Bus:/SFXBus");

    MasterBus = busMaps["Bus:/"];
    BGMbus = busMaps["Bus:/BGMBus"];
    SFXbus = busMaps["Bus:/SFXBus"];

    Set3dListenerAndOrientation(engine.GraphicsSys.camera);
    
    //EventID a = PlayEvent("event:/JumpSFX");
    //Set3DAudioEventPos(a, glm::vec3(20.0f, 40.0f, -80.0f));
   
	return true;
}
void AudioSystem::LoadBank(const char* bank_filename, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
    const char* word = FindWord(bank_filename);

    //IF already loaded, then return
    if (IsKeyInMap(bankMaps, word)) { return; }

    //ELSE load the bank
    FMOD::Studio::Bank* pBank;
    ERRCHECK(fmodStudioSystem->loadBankFile(Common_MediaPath(word), flags, &pBank));

    //IF load bank success, Load non-streaming sample data of the bank, store it in map
    if (pBank) 
    {
        bankMaps[word] = pBank; 
        pBank->loadSampleData();
    } 
}
void AudioSystem::UnloadBank(const char* bank_filename)
{
   
    auto pBank = IsKeyInMap(bankMaps, bank_filename);
    //IF bank isn't loaded, then return
    if (!pBank) { return; }

    //ELSE unload the bank, clear from map
    ERRCHECK(pBank->unloadSampleData());
    ERRCHECK(pBank->unload());
    bankMaps.erase(bank_filename);
}
void AudioSystem::LoadBus(const char* bus_filename)
{
    const char* word = FindWord(bus_filename);
    //IF already loaded, then return
    if (IsKeyInMap(busMaps, word)) { return; }

    //ELSE load the bus
    FMOD::Studio::Bus* pBus;
    ERRCHECK(fmodStudioSystem->getBus(word, &pBus));

    //IF load bus success, store it in map
    if (pBus)
    {
        busMaps[word] = pBus;
    }
}
void AudioSystem::UnloadBus(const char* bus_filename)
{

    auto pBus = IsKeyInMap(busMaps, bus_filename);
    //IF bus isn't loaded, then return
    if (!pBus) { return; }

    //ELSE clear from map
    busMaps.erase(bus_filename);
}

void AudioSystem::Update(float timeStamp)
{ 
    PROFILE_THIS("Audio Update");

    Set3dListenerAndOrientation(engine.GraphicsSys.camera);
    //MuteAll();
    //if (SFXMuted && BGMMuted) { return; }

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

    //TryPlayWaitingList();


    SetBusVolume("Bus:/BGMBus", static_cast<float>(BGMVolume));
    SetBusVolume("Bus:/SFXBus", static_cast<float>(SFXVolume));

  
    //printf(" %f, % f, %f", engine.GraphicsSys.camera.Position.x, engine.GraphicsSys.camera.Position.y, engine.GraphicsSys.camera.Position.z);

    ERRCHECK(fmodStudioSystem->update());
    
}
void AudioSystem::TryPlayWaitingList() 
{
    int size = waitingList.size(); 
    for (int i = 0; i < size; i++)
    {
        WaitingSound cur = waitingList.front();
        waitingList.pop_front();
        //fmodPlaySound(cur.strSoundName, cur.vPos, cur.fVolumedB);
    }
}
bool AudioSystem::Destroy()
{
    //UnloadBank("Master.bank");
    result = fmodStudioSystem->release();
    ERRCHECK(result);
    return !result;
}

void AudioSystem::LoadEvent(const char* event_filename)
{
    const char* word = FindWord(event_filename);

    //IF already loaded, then return
    if (IsKeyInMap(eventMaps, word)) { return; }

    //ELSE get eventDescription
    FMOD::Studio::EventDescription* pEventDescription = NULL;
    ERRCHECK(fmodStudioSystem->getEvent(word, &pEventDescription));

    //IF getEvent success, store it in map
    if (pEventDescription) 
    {
        eventMaps[word] = pEventDescription;
    }
}
EventID AudioSystem::PlayEvent(const char* event_filename)
{
    const char* word = FindWord(event_filename);

    //IF not event not loaded, load it
    if (!IsKeyInMap(eventMaps, word)) { LoadEvent(word); }

    //IF event still not loaded, then the event filename is wrong
    FMOD::Studio::EventDescription* pEventDescription = IsKeyInMap(eventMaps, word);
    if (!pEventDescription) { return -1; }

    //ELSE create an instance of the description
    FMOD::Studio::EventInstance* pEventInstance = NULL;
    ERRCHECK(pEventDescription->createInstance(&pEventInstance));

    //IF successfully created, store it in map, then play it, and return its ID
    if (pEventInstance) {
        std::pair<const char*, FMOD::Studio::EventInstance*>* pEventPair = new std::pair<const char*, FMOD::Studio::EventInstance*>(word, pEventInstance);
        eventInstanceMaps[currentEventID] = pEventPair;
        pEventInstance->start();
        return currentEventID++;
    }

    return -1;
}
void AudioSystem::StopEvent(const char* event_filename, bool bImmediate)
{
    //Find every instance that is the target event, then mute them
    for (auto it = eventInstanceMaps.begin(), itEnd = eventInstanceMaps.end(); it != itEnd; it++)
    {
        if (it->second->first)
        {
            FMOD_STUDIO_STOP_MODE eMode;
            eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
            ERRCHECK(it->second->second->stop(eMode));
        }
    }
}
void AudioSystem::StopEvent(EventID id, bool bImmediate)
{
    //IF eventInstance isn't in the map, then return
    auto pEventPair = IsKeyInMap(eventInstanceMaps, id);
    if (!pEventPair) { return; }

    //ELSE Stop the Instance
    FMOD_STUDIO_STOP_MODE eMode;
    eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
    ERRCHECK(pEventPair->second->stop(eMode));
}
bool AudioSystem::IsEventPlaying(EventID id) const
{
    //IF eventInstance isn't in the map, then return
    auto pEventPair = IsKeyInMap(eventInstanceMaps, id);
    if (!pEventPair) { return false; }

    //ELSE check if the Instance is playing
    FMOD_STUDIO_PLAYBACK_STATE state;
    ERRCHECK(pEventPair->second->getPlaybackState(&state));
    if (state == FMOD_STUDIO_PLAYBACK_PLAYING)
    {
        return true;
    }
    return false;
}
float AudioSystem::GetEventInstanceParameter(EventID id, const char* parameter_name)
{
    //IF eventInstance isn't in the map, then return
    auto pEventPair = IsKeyInMap(eventInstanceMaps, id);
    if (!pEventPair) { return NAN; }

    //ELSE get parameter value
    float* parameterValue = nullptr;
    ERRCHECK(pEventPair->second->getParameterByName(parameter_name, parameterValue));
    if (!parameterValue) { return NAN; }
    return *parameterValue;
}
void AudioSystem::SetEventInstanceParameter(EventID id, const char* parameter_name, float value)
{
    //IF eventInstance isn't in the map, then return
    auto pEventPair = IsKeyInMap(eventInstanceMaps, id);
    if (!pEventPair) { return; }

    //ELSE Set parameter
    ERRCHECK(pEventPair->second->setParameterByName(parameter_name, value));
}

void AudioSystem::SetBusMuted(FMOD::Studio::Bus* bus,bool isMuted)
{
    ERRCHECK(bus->setMute(isMuted));
}
void AudioSystem::SetBusMuted(const char* bus_name, bool isMuted)
{
    if (!IsKeyInMap(busMaps, bus_name)) { return; }
    ERRCHECK(busMaps[bus_name]->setMute(isMuted));
}
void AudioSystem::SetBusVolume(FMOD::Studio::Bus* bus, float fVolumedB)
{
    ERRCHECK(bus->setVolume(fVolumedB / 10.0f));
}
void AudioSystem::SetBusVolume(const char* bus_name, float fVolumedB)
{
    if (!IsKeyInMap(busMaps, bus_name)) { return; }
    ERRCHECK(busMaps[bus_name]->setVolume(fVolumedB / 10.0f));
}
void AudioSystem::MuteAll()
{
    MasterBus->setMute(true);
}

//void AudioSystem::LoadSound(const char* sound_filename, bool b3d, bool bLooping, bool BUS, bool bStream)
//{
//    auto mapIter = soundMaps.find(sound_filename);
//    if (mapIter != soundMaps.end())
//        return;
//
//    FMOD_MODE eMode = FMOD_DEFAULT;
//    eMode |= b3d ? FMOD_3D : FMOD_2D;
//    eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
//    eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
//    eMode |= FMOD_NONBLOCKING;
//
//    FMOD::Sound* pSound = nullptr;
//    ERRCHECK(coreSystem->createSound(Common_MediaPath(sound_filename), eMode, nullptr, &pSound));
//    if (pSound) {
//        soundMaps[sound_filename] = pSound;
//        groupMaps[sound_filename] = (int)BUS;
//    }
//}
//void AudioSystem::UnLoadSound(const char* sound_filename)
//{
//    auto mapIter = soundMaps.find(sound_filename);
//    if (mapIter == soundMaps.end()) { return; }
//    ERRCHECK(mapIter->second->release());
//    soundMaps.erase(mapIter);
//}
//int AudioSystem::fmodPlaySound(const char* sound_filename, const glm::vec3& vPos, float fVolumedB)
//{
//    int nChannelId = nextChannelId++;
//    auto mapIter = soundMaps.find(sound_filename);
//    if (mapIter == soundMaps.end())
//    {
//        LoadSound(sound_filename);
//        mapIter = soundMaps.find(sound_filename);
//        if (mapIter == soundMaps.end())
//        {
//            return nChannelId;
//        }
//    }
//    FMOD::Channel* pChannel = nullptr; 
//    FMOD_OPENSTATE openstate;
//    ERRCHECK((mapIter->second->getOpenState(&openstate, nullptr, nullptr, nullptr)));
//    //printf("%s state %d\n", sound_filename, (int)openstate);
//
//    if (openstate == FMOD_OPENSTATE_PLAYING) {}
//    else if (openstate != FMOD_OPENSTATE_READY&& openstate != FMOD_OPENSTATE_PLAYING) { waitingList.push_back(WaitingSound{ sound_filename ,vPos,fVolumedB }); return -1; }
//    
//    ERRCHECK(coreSystem->playSound(mapIter->second, nullptr, true, &pChannel));
//    if (pChannel)
//    {
//        FMOD_MODE currMode;
//        mapIter->second->getMode(&currMode);
//        if (currMode & FMOD_3D) {
//            FMOD_VECTOR position = vec3GLMtoFMOD(vPos);
//            ERRCHECK(pChannel->set3DAttributes(&position, nullptr));
//        }
//        ERRCHECK(pChannel->setVolume(dBtoVolume(fVolumedB)));
//        ERRCHECK(pChannel->setPaused(false));
//        channelMaps[nChannelId] = pChannel;
//        ERRCHECK(pChannel->setChannelGroup(groupMaps[sound_filename] ? BGM : SFX));
//    }
//    return nChannelId;
//}
void AudioSystem::Set3dListenerAndOrientation(Camera camera)
{
    FMOD_VECTOR pos = vec3GLMtoFMOD(camera.Position);
    FMOD_VECTOR orient;
    FMOD_VECTOR up;
    FMOD_VECTOR vel = vec3GLMtoFMOD(glm::vec3(0));

    glm::vec3 x = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 z = glm::vec3(0.0f, 0.0f, 1.0f);
    x = glm::rotate(x, -camera.yaw, y);
    z = glm::rotate(z, -camera.yaw, y);

    //std::cout << camera.yaw << camera.pitch << std::endl;

    //y = glm::rotate(, -camera.yaw, camera.Up);
    up = vec3GLMtoFMOD(y);
    orient = vec3GLMtoFMOD(-z);
    
    
    //coreSystem->set3DListenerAttributes(0, &pos, nullptr, &orient, &up);
    attribute.position = pos;
    attribute.velocity = vel;
    attribute.forward = orient;
    attribute.up = up;
    fmodStudioSystem->setListenerAttributes(0, &attribute);

    //coreSystem->get3DListenerAttributes(0, &p, nullptr, &f, &u);
    //std::cout << glm::to_string(camera.Position) << glm::to_string(-z) << glm::to_string(y) << std::endl;
    //ERRCHECK(coreSystem->set3DListenerAttributes(0, camera.Position, nullptr, camera.Orientation, camera.Up));
}
void AudioSystem::Set3DAudioEventPos(EventID id, glm::vec3 vPos)
{
    //IF eventInstance isn't in the map, then return
    auto pEventPair = IsKeyInMap(eventInstanceMaps, id);
    if (!pEventPair) { return; }

    //ELSE set 3D attribute
    FMOD_3D_ATTRIBUTES attributes;
    ERRCHECK(pEventPair->second->get3DAttributes(&attributes));
    attributes.position = vec3GLMtoFMOD(vPos);
    ERRCHECK(pEventPair->second->set3DAttributes(&attributes));
}
//void AudioSystem::SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition)
//{
//
//    auto mapIter = channelMaps.find(nChannelId);
//    if (mapIter == channelMaps.end()) { return; }
//
//    FMOD_VECTOR position = vec3GLMtoFMOD(vPosition);
//    ERRCHECK(mapIter->second->set3DAttributes(&position, NULL));
//}

//void AudioSystem::SetChannelVolume(int nChannelId, float fVolumedB)
//{
//    auto mapIter = channelMaps.find(nChannelId);
//    if (mapIter == channelMaps.end()) { return; }
//
//    ERRCHECK(mapIter->second->setVolume(dBtoVolume(fVolumedB)));
//}
//


//void AudioSystem::GetEventParameter(const char* event_filename, const char* parameter_name, float* value) {
//    auto mapIter = eventMaps.find(event_filename);
//    if (mapIter == eventMaps.end()) { return; }
//    ERRCHECK(mapIter->second->getParameterByName(parameter_name, value));
//
//
//    //    FMOD::Studio::ParameterInstance* pParameter = NULL;
//    //    CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
//    //    CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
//}


//void AudioSystem::SetEventParameter(const char* event_filename, const char* parameter_name, float value) {
//    auto mapIter = eventMaps.find(event_filename);
//    if (mapIter == eventMaps.end()) { return; }
//    ERRCHECK(mapIter->second->setParameterByName(parameter_name, value));
////    FMOD::Studio::ParameterInstance* pParameter = NULL;
////    CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
////    CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
//}



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


const char* AudioSystem::FindWord(const char* word)
{
    std::string stringWord(word);
    auto iter = std::find(wordDeque.begin(), wordDeque.end(), stringWord);
    if (iter != wordDeque.end())
    {
        return (*iter).c_str();
    }
    else
    {
        wordDeque.push_back(stringWord);
        return (wordDeque[wordDeque.size() - 1]).c_str();
    }
}

void AudioSystem::HandleEvent(Event event)
{
    if (event.type == EventType::MUTE_BGM)
    {
        SetBusMuted(BGMbus);
    }
    if (event.type == EventType::UNMUTE_BGM)
    {
        SetBusMuted(BGMbus, false);
    }
    if (event.type == EventType::MUTE_SFX)
    {
        SetBusMuted(SFXbus);
    }
    if (event.type == EventType::UNMUTE_SFX)
    {
        SetBusMuted(SFXbus, false);
    }
    if (event.type == EventType::PLAY_SOUND)
    {
        //PlayEvent("event:/BGM");
        //StopEvent(0,false);
        //fmodPlaySound(event.stringData1.c_str(), glm::vec3(event.floatData2, event.floatData3, event.floatData4), event.floatData1);
        EventID a = PlayEvent("event:/JumpSFX");
        Set3DAudioEventPos(a, glm::vec3(30.0f, 40.0f, -80.0f));
    }
}




