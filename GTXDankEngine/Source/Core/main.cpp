#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"



Engine engine;

#ifdef _DEBUG 
int main()
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
#endif
{


	Log::Init();	// this should be inside profiling system

	engine.Init();
	engine.Run();
	engine.Destroy();

	return 1;
}
