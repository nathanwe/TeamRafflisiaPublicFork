#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H

#include "pch.h"
#include "../Core/ComponentPool.h"
#include "../Systems/AudioSystem/AudioSystem.h"

class AudioComponent : public AbstractComponent
{
public:
	AudioComponent(Entity e);
};

extern AbstractComponentPool<AudioComponent> AudioComponentPool;

#endif 