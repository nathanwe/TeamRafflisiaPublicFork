#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

Engine engine;

int main()
{
	Log::Init();	// this should be inside profiling system

	engine.Init();
	engine.Run();
	engine.Destroy();

	return 1;
}
