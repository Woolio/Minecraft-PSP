#include <math.h>
#include <psptypes.h>

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>
#include <Aurora/Utils/Timer.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/Graphics/Camera.h>
#include <Aurora/Utils/StateManager.h>

#include "LameCraft/StateMenu.h"
#include "LameCraft/SplashScreen.h"
#include "LameCraft/SoundManager.h"
#include "LameCraft/TextureHelper.h"

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

PSP_MODULE_INFO("Tutorial1", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU|THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

int main()
{
	SetupCallbacks();

	scePowerSetClockFrequency(333, 333, 166);

	//initialize render manager
	RenderManager::InstancePtr()->Init();
	RenderManager::InstancePtr()->CollectPerformance(true);
	RenderManager::InstancePtr()->InitDebugFont();

	//set perspectives
	RenderManager::InstancePtr()->SetOrtho(0,0,0,0,0,0);

	//init and load sounds
	TextureHelper::Instance()->Init();
	SoundManager::Instance()->Init();

	srand(time(NULL));

	//new state manager
	StateManager stateManager;
	stateManager.Init();

	RenderManager::InstancePtr()->SetClearColour(0xFF000000);

	//splash screens
	SplashScreen *screen = new SplashScreen(TextureHelper::Instance()->GetTexture(TextureHelper::Genesis),0,0,480,272,3);
	screen->ShowSplash();
	delete screen;

	RenderManager::InstancePtr()->SetClearColour(0xFFFFFFFF);

	//new active state
	StateMenu *statePlay = new StateMenu();
	statePlay->Init();
	stateManager.ChangeState(statePlay);

	//trun
	while ( stateManager.Running() )
	{
		stateManager.HandleEvents();
		stateManager.Update();
		stateManager.Draw();
	}

	sceGuTerm();			// Terminating the Graphics System
	sceKernelExitGame();	// Quits Application

	return 0;
}
