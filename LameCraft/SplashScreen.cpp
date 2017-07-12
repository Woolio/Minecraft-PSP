#include "SplashScreen.h"

SplashScreen::SplashScreen(const char* filename,int startW,int startH,int endW,int endH,float splashTime)
{
	// load up the images
	sprite = new Sprite(filename,startW,startH,endW,endH);
	sprite->SetPosition(240,136);

	finishTime = splashTime;
	timer = 0.0f;
	dt = mTimer.GetDeltaTime();
}

SplashScreen::SplashScreen(int textureNumber,int startW,int startH,int endW,int endH,float splashTime)
{
	sprite = new Sprite(textureNumber,startW,startH,endW,endH);
	sprite->SetPosition(240,136);

	finishTime = splashTime;
	timer = 0.0f;
	dt = mTimer.GetDeltaTime();
}

SplashScreen::~SplashScreen()
{
	sprite->RemoveImage();
	delete sprite;
}


void SplashScreen::ShowSplash()
{
	dt = mTimer.GetDeltaTime();
	while(timer < finishTime)
	{
		dt = mTimer.GetDeltaTime();

		RenderManager::InstancePtr()->StartFrame(1,1,1);

		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1,1,1,1.0f));

		sprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		RenderManager::InstancePtr()->EndFrame();

		timer+=dt;
	}
	return;
}

