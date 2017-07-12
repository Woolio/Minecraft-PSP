#include <pge/pge.h>

/*
#include <minizip/unzip.h>
#include <errno.h>
#include <fcntl.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (512192)
#define MAXFILENAME (256)

int mymkdir(const char* dirname)
{
    int ret=0;
    ret = mkdir (dirname,777);
    return ret;
}

int makedir ( char *newdir)
{
  char *buffer ;
  char *p;
  int  len = (int)strlen(newdir);

  if (len <= 0)
    return 0;

  buffer = (char*)malloc(len+1);
        if (buffer==NULL)
        {
                printf("Error allocating memory\n");
                return UNZ_INTERNALERROR;
        }
  strcpy(buffer,newdir);

  if (buffer[len-1] == '/') {
    buffer[len-1] = '\0';
  }
  if (mymkdir(buffer) == 0)
    {
      free(buffer);
      return 1;
    }

  p = buffer+1;
  while (1)
    {
      char hold;

      while(*p && *p != '\\' && *p != '/')
        p++;
      hold = *p;
      *p = 0;
      if ((mymkdir(buffer) == -1) && (errno == ENOENT))
        {
          printf("couldn't create directory %s\n",buffer);
          free(buffer);
          return 0;
        }
      if (hold == 0)
        break;
      *p++ = hold;
    }
  free(buffer);
  return 1;
}

int do_extract_currentfile(unzFile uf,const int* popt_extract_without_path,int* popt_overwrite, const char* password)
{
    char filename_inzip[256];
    char* filename_withoutpath;
    char* p;
    int err=UNZ_OK;
    FILE *fout=NULL;
    void* buf;
    uInt size_buf;

    unz_file_info file_info;
    uLong ratio=0;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

    if (err!=UNZ_OK)
    {
        printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
        return err;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }

    if ((*filename_withoutpath)=='\0')
    {
        if ((*popt_extract_without_path)==0)
        {
            printf("creating directory: %s\n",filename_inzip);
            mymkdir(filename_inzip);
        }
    }
    else
    {
        const char* write_filename;
        int skip=0;

        if ((*popt_extract_without_path)==0)
            write_filename = filename_inzip;
        else
            write_filename = filename_withoutpath;

        err = unzOpenCurrentFilePassword(uf,password);
        if (err!=UNZ_OK)
        {
            printf("error %d with zipfile in unzOpenCurrentFilePassword\n",err);
        }

        if (((*popt_overwrite)==0) && (err==UNZ_OK))
        {
            char rep=0;
            FILE* ftestexist;
            ftestexist = fopen(write_filename,"rb");
            if (ftestexist!=NULL)
            {
                fclose(ftestexist);
            }

            *popt_overwrite=1;
        }

        if ((skip==0) && (err==UNZ_OK))
        {
            fout=fopen(write_filename,"wb");

            if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
                                (filename_withoutpath!=(char*)filename_inzip))
            {
                char c=*(filename_withoutpath-1);
                *(filename_withoutpath-1)='\0';
                makedir(write_filename);
                *(filename_withoutpath-1)=c;
                fout=fopen(write_filename,"wb");
            }

            if (fout==NULL)
            {
                printf("error opening %s\n",write_filename);
            }
        }

        if (fout!=NULL)
        {
            printf(" extracting: %s\n",write_filename);

            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err<0)
                {
                    printf("error %d with zipfile in unzReadCurrentFile\n",err);
                    break;
                }
                if (err>0)
                    if (fwrite(buf,err,1,fout)!=1)
                    {
                        printf("error in writing extracted file\n");
                        err=UNZ_ERRNO;
                        break;
                    }
            }
            while (err>0);
            if (fout)
                    fclose(fout);

        }

        if (err==UNZ_OK)
        {
            err = unzCloseCurrentFile (uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzCloseCurrentFile\n",err);
            }
        }
        else
            unzCloseCurrentFile(uf); 
    }

    free(buf);
    return err;
}


int do_extract(unzFile uf,int opt_extract_without_path,int opt_overwrite,const char* password)
{
    uLong i;
    unz_global_info gi;
    int err;
    FILE* fout=NULL;

    err = unzGetGlobalInfo(uf,&gi);
    if (err!=UNZ_OK)
        printf("error %d with zipfile in unzGetGlobalInfo \n",err);

    for (i=0;i<gi.number_entry;i++)
    {
        if (do_extract_currentfile(uf,&opt_extract_without_path,&opt_overwrite,password) != UNZ_OK)
            break;

        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }

    return 0;
}*/

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
	
	/*unzFile uf=NULL;
	
	
	uf = unzOpen("../Lamecraft.zip");
	if(uf == NULL)
	{
		pspDebugScreenPrintf("Can't open lamecraft.zip");
	}
	
	chdir("..");
	
	do_extract(uf, 0, 1, NULL);
	
	remove("Lamecraft.zip");*/
	
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
