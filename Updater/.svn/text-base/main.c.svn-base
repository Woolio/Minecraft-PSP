#include <pge/pge.h>

int main(int argc, char*argv[])
{
	pgeGfxInit(PGE_PIXEL_FORMAT_8888);
	pgeControlsInit();
	
	pgeNetInit();
	pgeUtilsNetDialogInit();
	
	while(pgeRunning())
	{
		pgeGfxStartDrawing();
		
		pgeGfxClearScreen(0);
		
		pgeGfxEndDrawing();
		
		int result = pgeUtilsNetDialogUpdate();
		
		if(result != -1)
			break;
			
		pgeGfxSwapBuffers(PGE_WAIT_VSYNC);
	}
	
	pspDebugScreenInit();
	
	pspDebugScreenSetXY(0, 2);
	pspDebugScreenPrintf("Downloading update...");
	
	//download file
	pgeNetGetFile("http://drakon.ixan.net/psp/Lamecraft.zip", "../Lamecraft.zip");
	
	//installing file
	pspDebugScreenSetXY(0, 4);
	pspDebugScreenPrintf("Installing update...");
	
	
	pgeZip* zipFiles = pgeZipOpen("../Lamecraft.zip");
	
	chdir("..");
	
	pgeZipExtract(zipFiles, NULL);
	pgeZipClose(zipFiles);
	pgeFileRemove("Lamecraft.zip");
	
	pspDebugScreenSetXY(0, 6);
	pspDebugScreenPrintf("Installation done - press X to exit");
	
	while(1)
	{
		pgeControlsUpdate();
		if(pgeControlsHeld(PGE_CTRL_CROSS))
		{
			break;
		}
	}
	
	pgeNetShutdown();
	pgeExit();
	return 0;
}
