#include "oslib.h"

#include <mikmod.h>
#include <mmio.h>

static int osl_modPlaying = 0;
static int osl_modInitialized = 0;
static int osl_modFrequency = 44100, osl_modShift = 0, osl_modStereo = 1;

static BOOL PSP_IsThere(void)
{
	return 1;
}

/*static void sound_callback(void *buf, unsigned int reqn)
{
	if (osl_modPlaying)
	{
		VC_WriteBytes(buf, reqn*4);
	}
	else
	{
		memset(buf, 0, reqn*4);
	}
}*/

static BOOL PSP_Init(void)
{
	if (VC_Init())
		return 1;

//	pspAudioInit();
//	pspAudioSetChannelCallback(0, (void *)sound_callback);

	return 0;
}

static void PSP_Exit(void)
{
//	pspAudioEndPre();
//	pspAudioEnd();
	VC_Exit();
}

static void PSP_Update(void)
{
}

static BOOL PSP_Reset(void)
{
	VC_Exit();
	return VC_Init();
}

static BOOL PSP_PlayStart(void)
{
	VC_PlayStart();
	osl_modPlaying = 1;
	return 0;
}

static void PSP_PlayStop(void)
{
	osl_modPlaying = 0;
	VC_PlayStop();
}

MDRIVER drv_psp =
{
	NULL,
	"PSP Audio",
	"PSP Output Driver v1.0 - by Jim Shaw",
	0,255,
	PSP_IsThere,
	(void*)VC_SampleLoad,
	VC_SampleUnload,
	VC_SampleSpace,
	VC_SampleLength,
	PSP_Init,
	PSP_Exit,
	PSP_Reset,
	VC_SetNumVoices,
	PSP_PlayStart,
	PSP_PlayStop,
	PSP_Update,
	VC_VoiceSetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceSetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceReleaseSustain,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
};

#if 0
static void MODStop(OSL_SOUND *s)		{
	UNIMOD *mod = (UNIMOD*)s->data;
//	osl_numberOfModsPlaying--;
//    if (osl_numberOfModsPlaying <= 0) MikMod_DisableOutput();
    mod->forbid = 1;
}

static int MODActive(OSL_SOUND *s)		{
	UNIMOD *mod = (UNIMOD*)s->data;
	return (!(mod->sngpos >= mod->numpos));
}

static void MODStart(OSL_SOUND *s)		{
	UNIMOD *mod = (UNIMOD*)s->data;

    if(!MikMod_Active())
    {
		//isfirst = 2;
        MikMod_EnableOutput();
    }

    mod->forbid = 0;
/*    if(pf != mf)
    {   // new song is being started, so completely stop out the old one.
        if(pf!=NULL) pf->forbid = 1;
        for(t=0; t<md_sngchn; t++) Voice_Stop(t);
    }

    pf = mf;*/
}
#endif

/*
	Callbacks standard
*/
void oslAudioCallback_PlaySound_MOD(OSL_SOUND *s)		{
	//Stereo only, mono doesn't work yet (and the speedup wouldn't be worth it)
	//md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_MUSIC;
	s->mono = 0;

/*	int oldmode = md_mode;
	if (osl_modStereo)		{
		md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_MUSIC;
		s->mono = 0;
	}
	else		{
		md_mode = DMODE_16BITS|DMODE_SOFT_MUSIC;
		s->mono = 0x10;
	}

	if (md_mode != oldmode)			{
		int voice = oslGetSoundChannel(s);
		MikMod_Reset();
		if (voice >= 0)
			oslAudioRecreateChannel(voice, s->mono);
	}*/

	Player_Stop();
	Player_Start((UNIMOD*)s->data);
	Player_SetPosition(0);
}

void oslAudioCallback_StopSound_MOD(OSL_SOUND *s)		{
	Player_Stop();
}

int oslAudioCallback_AudioCallback_MOD(unsigned int i, void* buf, unsigned int length)			{
	//Set up playback
	//md_mixfreq = osl_modFrequency;
	//md_mixshift = osl_modShift;

	if (osl_modPlaying)
		VC_WriteBytes(buf, length << 2);
	else
		memset(buf, 0, length << 2);

	//Fin de la chanson ^^
	if (!Player_Active())
		return 0;
	return 1;
}

VIRTUAL_FILE **oslAudioCallback_ReactiveSound_MOD(OSL_SOUND *s, VIRTUAL_FILE *f)			{
	return NULL;
}

VIRTUAL_FILE *oslAudioCallback_StandBy_MOD(OSL_SOUND *s)		{
	return NULL;
}

void oslAudioCallback_DeleteSound_MOD(OSL_SOUND *s)		{
//	Player_Stop();
	MikMod_FreeSong((UNIMOD*)s->data);
}

void my_error_handler(void)
{
	//oslDebug("_mm_critical %i\nmm_errno %i\n%s", _mm_critical, _mm_errno, _mm_errmsg[_mm_errno]);
	return;
}

/*
	Exemple:
	oslSetModSampleRate(11025, 2);				//Very low CPU, bad sound
	oslSetModSampleRate(22050, 1);				//Low CPU, medium sound
	oslSetModSampleRate(44100, 0);				//Normal CPU usage, good sound

	oslSetModSampleRate(22050, 0);				//Twice the normal speed
*/
void oslSetModSampleRate(int freq, int stereo, int shift)		{
	osl_modFrequency = freq;
	osl_modStereo = stereo;
	osl_modShift = shift;
}

OSL_SOUND *oslLoadSoundFileMOD(const char *filename, int stream)		{
	OSL_SOUND *s;
	UNIMOD *mf;

	if (!osl_modInitialized)		{
//		_mm_RegisterErrorHandler(my_error_handler);
		MikMod_RegisterAllLoaders();
		MikMod_RegisterDriver(drv_psp);

		//md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_MUSIC;
		MikMod_Init();

		osl_modInitialized = 1;
	}

	s = (OSL_SOUND*)malloc(sizeof(OSL_SOUND));
	if (s)			{
		//Never forget that! If any member is added to OSL_SOUND, it is assumed to be zero!
		memset(s, 0, sizeof(OSL_SOUND));
		mf = MikMod_LoadSong((char*)filename, 128);
		if (mf)		{
			s->data = (void*)mf;

			s->endCallback = NULL;
			s->volumeLeft = s->volumeRight = OSL_VOLUME_MAX;
			//No special format
			s->format = 0;
			//Always stereo output
			s->mono = 0;
			s->divider = OSL_FMT_44K;
			//MOD files are never streamed
			s->isStreamed = 0;
			//Use the default value
			s->numSamples = 0;

		/*	if (wav->fmt.sample_rate >= 44100)
				s->divider = OSL_FMT_44K;
			else if (wav->fmt.sample_rate >= 22050)
				s->divider = OSL_FMT_22K;
			else
				s->divider = OSL_FMT_11K;*/

			s->audioCallback = oslAudioCallback_AudioCallback_MOD;
			s->playSound = oslAudioCallback_PlaySound_MOD;
			s->stopSound = oslAudioCallback_StopSound_MOD;
			s->standBySound = oslAudioCallback_StandBy_MOD;
			s->reactiveSound = oslAudioCallback_ReactiveSound_MOD;
			s->deleteSound = oslAudioCallback_DeleteSound_MOD;
		}
		else		{
			free(s);
			s = NULL;
		}
	}

	if (!s)
	{
		//oslHandleLoadNoFailError(filename);
		}
	return s;

/*	VirtualFileRead(&bfh, sizeof(bfh), 1, f);
	//Vérifie l'en-tête
	if (strcmp(bfh.strVersion, "OSLBGM v01"))		{
		free(s);
		VirtualFileClose(f);
		return NULL;
	}
	if (bfh.format == 1)		{
		//Pour l'adpcm, dataplus contient la structure définissant les données
		ad = (OSL_ADGlobals*)malloc(sizeof(OSL_ADGlobals));
		if (!ad)		{
			free(s);
			VirtualFileClose(f);
			return NULL;
		}
		s->dataplus = ad;
	}
	debut = VirtualFileTell(f);
	s->baseoffset = debut;
	VirtualFileSeek(f, 0, SEEK_END);
	fin = VirtualFileTell(f);
	if (fin-debut <= 0)			{
		free(s);
		if (ad)	free(ad);
		VirtualFileClose(f);
		return NULL;
	}
	VirtualFileSeek(f, 0, SEEK_SET);
	s->isStreamed = stream;
	if (s->isStreamed)			{
		if (strlen(filename) >= sizeof(s->filename))
			oslFatalError("Sound file name too long!");
		strcpy(s->filename, filename);
		s->suspendNumber = osl_suspendNumber;
		s->data = (void*)f;
	}
	else		{
		s->data = malloc(fin-debut);
		if (!s->data)		{
			free(s);
			if (ad)	free(ad);
			VirtualFileClose(f);
			return NULL;
		}
		VirtualFileRead(s->data, fin-debut, 1, f);
		VirtualFileClose(f);
	}
	s->endCallback = NULL;
	s->format = bfh.format;
	if (bfh.sampleRate == 44100)
		s->divider = OSL_FMT_44K;
	else if (bfh.sampleRate == 22050)
		s->divider = OSL_FMT_22K;
	else if (bfh.sampleRate == 11025)
		s->divider = OSL_FMT_11K;
	s->size = fin-debut;
//	if (bfh.format & OSL_FMT_STEREO)
//		s->mono = 0;				//OSL_AUDIO_FORMAT_STEREO
//	else
		s->mono = 0x10;				//OSL_AUDIO_FORMAT_MONO
	s->volumeLeft = s->volumeRight = OSL_VOLUME_MAX;

	s->audioCallback = oslAudioCallback_AudioCallback_BGM;
	s->playSound = oslAudioCallback_PlaySound_BGM;
	s->standBySound = oslAudioCallback_StandBy_BGM;
	s->reactiveSound = oslAudioCallback_ReactiveSound_BGM;
	s->deleteSound = oslAudioCallback_DeleteSound_BGM;

	return s;*/
}


