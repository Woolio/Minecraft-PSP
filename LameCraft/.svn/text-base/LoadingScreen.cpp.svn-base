#include "LoadingScreen.h"
#include <pspthreadman.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/Sprite.h>

using namespace Aurora::Graphics;

Sprite* backSprite;
LoadingScreen::LoadingScreen()
{
	// the loadingscreen is loaded as a thread
	thid_ = sceKernelCreateThread("LoadingThread", RunLoadingScreen, 0x18, 0x10000, THREAD_ATTR_VFPU|THREAD_ATTR_USER, NULL);
	// start the thread
	sceKernelStartThread(thid_, 0, 0);
}


void LoadingScreen::KillLoadingScreen()
{
	// shut down the loading screen again.
	sceKernelTerminateDeleteThread(thid_);
	// free the mem space of the images
	delete backSprite;
}


int LoadingScreen::RunLoadingScreen(SceSize args, void *argp)
{
	// load up the images
	backSprite = new Sprite("Assets/Lamecraft/dirt.png",0,0,16,16);
	backSprite->Scale(2,2);


	// start the render loop
	while(1)
	{
		RenderManager::InstancePtr()->StartFrame();

		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1,1,1,1.0f));

		for(int x = 0;x < 16;x++)
		{
			for(int y = 0;y < 9;y++)
			{
				backSprite->SetPosition(x*32,y*32);
				backSprite->Draw();
			}
		}

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		//draw subtitles on buttons

		RenderManager::InstancePtr()->DebugPrint(240,136,"GENERATING WORLD");

		RenderManager::InstancePtr()->EndFrame();
	}
	return 0;
}

