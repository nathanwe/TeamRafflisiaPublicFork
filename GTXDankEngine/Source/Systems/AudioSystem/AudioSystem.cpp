#include "pch.h"
#include "AudioSystem.h"
#include "../Core/Engine.h"
#include "../utils/common.h"


bool AudioSystem::Init()
{
    system = NULL;
    ERRCHECK(FMOD::Studio::System::create(&system));

    // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
    FMOD::System* coreSystem = NULL;
    ERRCHECK(system->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

    ERRCHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    FMOD::Studio::Bank* masterBank = NULL;
    ERRCHECK(system->loadBankFile(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));

    FMOD::Studio::Bank* stringsBank = NULL;
    ERRCHECK(system->loadBankFile(Common_MediaPath("Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK(system->getEvent("event:/BGM", &eventDescription));

    FMOD::Studio::EventInstance* eventInstance = NULL;
    ERRCHECK(eventDescription->createInstance(&eventInstance));

    FMOD::Studio::Bus* masterBus = NULL;
    ERRCHECK(system->getBus("bus:/", &masterBus));

    masterBus->setVolume(1.0f);

    eventInstance->start();
    eventInstance->setVolume(0.1f);

	return true;
}

void AudioSystem::Update(float timeStamp)
{
    system->update();
}

bool AudioSystem::Destroy()
{
    result = system->release();
    ERRCHECK(result);
    return !result;
}





