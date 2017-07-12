#include "SoundManager.h"

using namespace Aurora;

SoundManager SoundManager::m_SoundManager;

SoundManager* SoundManager::Instance()
{
	return &m_SoundManager;
}

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
    pgeWavStopAll();

    pgeWavUnload(rainSound);
    pgeWavUnload(buttonSound);
    pgeWavUnload(tnt);
    pgeWavUnload(doorOpen);
    pgeWavUnload(doorClose);
    pgeWavUnload(breakSound);
    pgeWavUnload(bow);
    pgeWavUnload(plopSound);
    pgeWavUnload(zombieDieSound);
    pgeWavUnload(cowSaySound);
    pgeWavUnload(fuse);
    pgeWavUnload(fiss);
    pgeWavUnload(splash);
    pgeWavUnload(glass);
    pgeWavUnload(zombieSaySound);
    pgeWavUnload(shear);

    for(int i = 0; i <= 3; i++)
    {
        pgeWavUnload(grassSounds[i]);
        pgeWavUnload(grawelSounds[i]);
        pgeWavUnload(stoneSounds[i]);
        pgeWavUnload(woodSounds[i]);
        pgeWavUnload(sandSounds[i]);
        pgeWavUnload(snowSounds[i]);
    }
    for(int i = 0; i <= 2; i++)
    {
        pgeWavUnload(eatSounds[i]);
        pgeWavUnload(hitSounds[i]);
        pgeWavUnload(digSounds[i]);
    }
    for(int i = 0; i <= 1; i++)
    {
        pgeWavUnload(fallSounds[i]);
        pgeWavUnload(cowHurtSounds[i]);
        pgeWavUnload(creeperHurtSounds[i]);
        pgeWavUnload(zombieHurtSounds[i]);
        pgeWavUnload(sheepHurtSounds[i]);
        pgeWavUnload(noteSounds[i]);
    }
}

void SoundManager::Init()
{
    //first of all init audio engine
    VirtualFileInit();
   // oslInitAudioME(OSL_FMT_MP3);
	oslInitAudio();
	pgeWavInit();

    //set default values
	currentWalkSound = 0;
	lastWalkSound = 0;
	playerSounds = true;
	lastAmbientSound = 0;
	currentAmbientSound = 0;
	ambientSoundsEnabled = true;
	srand(time(NULL));

	//button sound
	buttonSound = pgeWavLoad("Assets/Sounds/other/button1.wav");

	//TNT
	tnt = pgeWavLoad("Assets/Sounds/other/tnt.wav");
	fuse = pgeWavLoad("Assets/Sounds/other/fuse.wav");
    fiss = pgeWavLoad("Assets/Sounds/other/fizz.wav");
	splash = pgeWavLoad("Assets/Sounds/other/splash.wav");

	doorOpen = pgeWavLoad("Assets/Sounds/other/door_open.wav");
	doorClose = pgeWavLoad("Assets/Sounds/other/door_close.wav");

	breakSound = pgeWavLoad("Assets/Sounds/other/break.wav");

	rainSound = pgeWavLoad("Assets/Sounds/other/rain.wav");

	bow = pgeWavLoad("Assets/Sounds/other/bow.wav");

	shear = pgeWavLoad("Assets/Sounds/mobs/sheep/shear.wav");

	//plop sound
	plopSound = pgeWavLoad("Assets/Sounds/other/plop.wav");
	pgeWavVolume(plopSound,50,50);

    //fall sounds
    fallSounds[0] = pgeWavLoad("Assets/Sounds/Damage/fallbig.wav");
    fallSounds[1] = pgeWavLoad("Assets/Sounds/Damage/fallsmall.wav");

    //note sounds
    noteSounds[0] = pgeWavLoad("Assets/Sounds/notes/bass.wav");
    noteSounds[1] = pgeWavLoad("Assets/Sounds/notes/pling.wav");
    pgeWavVolume(noteSounds[0],85,85);
    pgeWavVolume(noteSounds[1],85,85);

    //zombie sounds
    zombieHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/zombie/hurt1.wav");
    zombieHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/zombie/hurt2.wav");
    zombieSaySound = pgeWavLoad("Assets/Sounds/mobs/zombie/say.wav");
    zombieDieSound = pgeWavLoad("Assets/Sounds/mobs/zombie/death.wav");

    //cow sounds
    cowHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/cow/hurt1.wav");
    cowHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/cow/hurt2.wav");
    cowSaySound = pgeWavLoad("Assets/Sounds/mobs/cow/say1.wav");

    //creeper sounds
    creeperHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/creeper/hurt1.wav");
    creeperHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/creeper/hurt2.wav");

    //sheep sounds
    sheepHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/sheep/say1.wav");
    sheepHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/sheep/say2.wav");

	//grass sounds
	grassSounds[0] = pgeWavLoad("Assets/Sounds/walking/grass/grass1.wav");
	grassSounds[1] = pgeWavLoad("Assets/Sounds/walking/grass/grass2.wav");
	grassSounds[2] = pgeWavLoad("Assets/Sounds/walking/grass/grass3.wav");
	grassSounds[3] = pgeWavLoad("Assets/Sounds/walking/grass/grass4.wav");

	//gravel
	grawelSounds[0] = pgeWavLoad("Assets/Sounds/walking/gravel/gravel1.wav");
	grawelSounds[1] = pgeWavLoad("Assets/Sounds/walking/gravel/gravel2.wav");
	grawelSounds[2] = pgeWavLoad("Assets/Sounds/walking/gravel/gravel3.wav");
	grawelSounds[3] = pgeWavLoad("Assets/Sounds/walking/gravel/gravel4.wav");

	//stone
	stoneSounds[0] = pgeWavLoad("Assets/Sounds/walking/stone/stone1.wav");
	stoneSounds[1] = pgeWavLoad("Assets/Sounds/walking/stone/stone2.wav");
	stoneSounds[2] = pgeWavLoad("Assets/Sounds/walking/stone/stone3.wav");
	stoneSounds[3] = pgeWavLoad("Assets/Sounds/walking/stone/stone4.wav");

	//wood
	woodSounds[0] = pgeWavLoad("Assets/Sounds/walking/wood/wood1.wav");
	woodSounds[1] = pgeWavLoad("Assets/Sounds/walking/wood/wood2.wav");
	woodSounds[2] = pgeWavLoad("Assets/Sounds/walking/wood/wood3.wav");
	woodSounds[3] = pgeWavLoad("Assets/Sounds/walking/wood/wood4.wav");

    //sand
    sandSounds[0] = pgeWavLoad("Assets/Sounds/walking/sand/sand1.wav");
	sandSounds[1] = pgeWavLoad("Assets/Sounds/walking/sand/sand2.wav");
	sandSounds[2] = pgeWavLoad("Assets/Sounds/walking/sand/sand3.wav");
	sandSounds[3] = pgeWavLoad("Assets/Sounds/walking/sand/sand4.wav");

    //snow
    snowSounds[0] = pgeWavLoad("Assets/Sounds/walking/snow/snow1.wav");
	snowSounds[1] = pgeWavLoad("Assets/Sounds/walking/snow/snow2.wav");
	snowSounds[2] = pgeWavLoad("Assets/Sounds/walking/snow/snow3.wav");
	snowSounds[3] = pgeWavLoad("Assets/Sounds/walking/snow/snow4.wav");

	//glass
	glass = pgeWavLoad("Assets/Sounds/dig/glass1.wav");

	digSounds[0] = pgeWavLoad("Assets/Sounds/dig/grass1.wav");
	digSounds[1] = pgeWavLoad("Assets/Sounds/dig/gravel1.wav");
	digSounds[2] = pgeWavLoad("Assets/Sounds/dig/stone1.wav");

    // Eat sounds
	eatSounds[0] = pgeWavLoad("Assets/Sounds/eat/eat1.wav");
	eatSounds[1] = pgeWavLoad("Assets/Sounds/eat/eat2.wav");
	eatSounds[2] = pgeWavLoad("Assets/Sounds/eat/eat3.wav");

    // Hit sounds
    hitSounds[0] = pgeWavLoad("Assets/Sounds/Damage/hit1.wav");
    hitSounds[1] = pgeWavLoad("Assets/Sounds/Damage/hit2.wav");
    hitSounds[2] = pgeWavLoad("Assets/Sounds/Damage/hit3.wav");

    ambientTrack = NULL;
    ambientTrackName = " ";

    diskTrack = NULL;
    diskTrackName = " ";

    delayAmbient = false;
}

void SoundManager::AudioSync()
{
    // не в том проблема надо оставить
    oslAudioVSync();
}

void SoundManager::PlayMenuSound()
{
	pgeWavPlay(buttonSound);
}

void SoundManager::PlayPlopSound()
{
	if(playerSounds)
		pgeWavPlay(plopSound);
}

void SoundManager::TNTSound()
{
	if(playerSounds)
		pgeWavPlay(tnt);
}

void SoundManager::PlayFuseSound()
{
	if(playerSounds)
		pgeWavPlay(fuse);
}

void SoundManager::PlayFissSound()
{
	if(playerSounds)
		pgeWavPlay(fiss);
}

void SoundManager::PlayShearSound()
{
	if(playerSounds)
		pgeWavPlay(shear);
}

void SoundManager::PlaySplashSound()
{
	if(playerSounds)
    {
        int volume = 88+rand()%10;
        pgeWavVolume(splash,volume,volume);
		pgeWavPlay(splash);
    }
}

void SoundManager::PlayNoteSound(unsigned int noteType, float pitch)
{
	if(playerSounds)
    {
        pgeWavPitch(noteSounds[noteType],pitch);
        pgeWavPlay(noteSounds[noteType]);
        pgeWavPitch(noteSounds[noteType],1.0f);
    }
}

void SoundManager::PlayDoorSound(bool open)
{
	if(playerSounds)
    {
        open == true ? pgeWavPlay(doorOpen) : pgeWavPlay(doorClose);
    }
}

void SoundManager::PlayGlassSound()
{
	if(playerSounds)
		pgeWavPlay(glass);
}

void SoundManager::PlayRainSound(int volume)
{
	if(playerSounds)
    {
        pgeWavVolume(rainSound, volume, volume);
        pgeWavPlay(rainSound);
        pgeWavVolume(rainSound, 100, 100);
    }
}

void SoundManager::PlayEndDigSound(int type)
{
	if(playerSounds)
	{
		switch(type)
		{
			case 0://grass
			{
				pgeWavPlay(digSounds[0]);
			}
			break;
			case 1://gravel
			{
				pgeWavPlay(digSounds[1]);
			}
			break;
			case 2://stone
			{
				pgeWavPlay(digSounds[2]);
			}
			break;
			case 3://wood
			{
				pgeWavPlay(woodSounds[rand() % 4]);
			}
			break;
            case 4://cloth
			{
			    pgeWavPlay(snowSounds[rand() % 4]);
			}
			break;
            case 5://sand
			{
				pgeWavPlay(sandSounds[rand() % 4]);
			}
			break;
            case 6://snow
			{
				pgeWavPlay(snowSounds[rand() % 4]);
			}
			break;
		}
	}
}

void SoundManager::PlayBreakSound()
{
	if(playerSounds)
		pgeWavPlay(breakSound);
}

void SoundManager::PlayBowSound()
{
	if(playerSounds)
		pgeWavPlay(bow);
}

void SoundManager::PlayZombieHurtSound(float distanceToPlayer)
{
	if(playerSounds)
    {
        int i = rand() % 2;
        if(distanceToPlayer < 15.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (15.0f-distanceToPlayer)/15.0f*100.0f;
            pgeWavVolume(zombieHurtSounds[i],volume,volume);
            pgeWavPlay(zombieHurtSounds[i]);
        }
    }
}

void SoundManager::PlayZombieDieSound(float distanceToPlayer)
{
	if(playerSounds)
    {
        if(distanceToPlayer < 15.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (15.0f-distanceToPlayer)/15.0f*100.0f;
            pgeWavVolume(zombieDieSound,volume,volume);
            pgeWavPlay(zombieDieSound);
        }
    }
}

void SoundManager::PlayCowHurtSound(float distanceToPlayer)
{
	if(playerSounds)
    {
        int i = rand()%2;
        if(distanceToPlayer < 15.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (15.0f-distanceToPlayer)/15.0f*100.0f;
            pgeWavVolume(cowHurtSounds[i],volume,volume);
            pgeWavPlay(cowHurtSounds[i]);
        }
    }
}

void SoundManager::PlayCreeperHurtSound(float distanceToPlayer)
{
	if(playerSounds)
    {
        int i = rand()%2;
        if(distanceToPlayer < 15.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (15.0f-distanceToPlayer)/15.0f*100.0f;
            pgeWavVolume(creeperHurtSounds[i],volume,volume);
            pgeWavPlay(creeperHurtSounds[i]);
        }
    }
}

void SoundManager::PlaySheepHurtSound(float distanceToPlayer)
{
	if(playerSounds)
    {
        int i = rand()%2;
        if(distanceToPlayer < 15.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (15.0f-distanceToPlayer)/15.0f*100.0f;
            pgeWavVolume(sheepHurtSounds[i],volume,volume);
            pgeWavPlay(sheepHurtSounds[i]);
        }
    }
}

void SoundManager::PlayZombieSaySound(float distanceToPlayer)
{
	if(playerSounds)
    {
        if(distanceToPlayer < 22.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (22.0f-distanceToPlayer)/22.0f*100.0f;
            pgeWavVolume(zombieSaySound,volume,volume);
            pgeWavPlay(zombieSaySound);
        }
    }
}

void SoundManager::PlayCowSaySound(float distanceToPlayer)
{
	if(playerSounds)
    {
        if(distanceToPlayer < 20.0f)
        {
            distanceToPlayer -= 2.0f;
            if(distanceToPlayer < 0)
            {
                distanceToPlayer = 0.0f;
            }
            float volume = (20.0f-distanceToPlayer)/20.0f*100.0f;
            pgeWavVolume(cowSaySound,volume,volume);
            pgeWavPlay(cowSaySound);
        }
    }
}

void SoundManager::PlayHitSound()
{
	if(playerSounds)
    {
		pgeWavPlay(hitSounds[rand() % 3]);
    }
}

void SoundManager::PlayEatSound()
{
	if(playerSounds)
    {
		pgeWavPlay(eatSounds[rand() % 3]);
    }
}


void SoundManager::PlayFallSound(float i)
{
	if(playerSounds)
    {
        if (i < -9 && i > -21)
        {
            pgeWavPlay(fallSounds[1]);
        }
        if (i < -21)
        {
            pgeWavPlay(fallSounds[0]);
        }
    }
}

void SoundManager::PlayWalkSound(int type)
{
	if(playerSounds)
	{
		switch(type)
		{
			case 0://grass
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(grassSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(grassSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
			case 1://gravel
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(grawelSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(grawelSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
			case 2://stone
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(stoneSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(stoneSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
			case 3://wood
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(woodSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(woodSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
            case 4://cloth
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(snowSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(snowSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
            case 5://sand
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(sandSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(sandSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
            case 6://snow
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(snowSounds[lastWalkSound]);

				//play new sound
				pgeWavPlay(snowSounds[currentWalkSound]);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
		}
	}
}

void SoundManager::PlayDigSound(int type)
{
	if(playerSounds)
	{
		switch(type)
		{
			case 0://grass
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(grassSounds[lastWalkSound]);
				pgeWavVolume(grassSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(grassSounds[currentWalkSound],25,25);
				pgeWavPlay(grassSounds[currentWalkSound]);
				pgeWavVolume(grassSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
			case 1://gravel
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(grawelSounds[lastWalkSound]);
				pgeWavVolume(grawelSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(grawelSounds[currentWalkSound],25,25);
				pgeWavPlay(grawelSounds[currentWalkSound]);
				pgeWavVolume(grawelSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
			case 2://stone
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(stoneSounds[lastWalkSound]);
				pgeWavVolume(stoneSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(stoneSounds[currentWalkSound],25,25);
				pgeWavPlay(stoneSounds[currentWalkSound]);
				pgeWavVolume(stoneSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
			case 3://wood
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(woodSounds[lastWalkSound]);
				pgeWavVolume(woodSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(woodSounds[currentWalkSound],25,25);
				pgeWavPlay(woodSounds[currentWalkSound]);
				pgeWavVolume(woodSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
            case 4://cloth
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(snowSounds[lastWalkSound]);
				pgeWavVolume(snowSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(snowSounds[currentWalkSound],25,25);
				pgeWavPlay(snowSounds[currentWalkSound]);
				pgeWavVolume(snowSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
            case 5://sand
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(sandSounds[lastWalkSound]);
				pgeWavVolume(sandSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(sandSounds[currentWalkSound],25,25);
				pgeWavPlay(sandSounds[currentWalkSound]);
				pgeWavVolume(sandSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
            case 6://snow
			{
				currentWalkSound = rand() % 4;

				//stop last sound
				pgeWavStop(snowSounds[lastWalkSound]);
				pgeWavVolume(snowSounds[lastWalkSound],100,100);

				//play new sound
				pgeWavVolume(snowSounds[currentWalkSound],25,25);
				pgeWavPlay(snowSounds[currentWalkSound]);
				pgeWavVolume(snowSounds[currentWalkSound],100,100);

				//set last sound as current
				lastWalkSound = currentWalkSound;
			}
			break;
		}
	}
}

void SoundManager::StopAmbient()
{
    if(ambientTrack != NULL)
    {
        oslStopSound(ambientTrack);
        oslDeleteSound(ambientTrack);
        ambientTrack = NULL;

        ambientTrackName = " ";
    }
}

int SoundManager::PlayRandomAmbient()
{
 	switch(rand() % 12)
 	{
 	    case 0: PlayAmbient("calm1"); break;
 	    case 1: PlayAmbient("calm2"); break;
 	    case 2: PlayAmbient("calm3"); break;
 	    case 3: PlayAmbient("piano1"); break;
 	    case 4: PlayAmbient("piano2"); break;
 	    case 5: PlayAmbient("piano3"); break;
 	    case 6: PlayAmbient("nuance1"); break;
 	    case 7: PlayAmbient("nuance2"); break;
 	    case 8: PlayAmbient("hal1"); break;
 	    case 9: PlayAmbient("hal2"); break;
 	    case 10: PlayAmbient("hal3"); break;
 	    case 11: PlayAmbient("hal4"); break;
 	}
    int delay = 540+rand()%121;
	return delay;
}

void SoundManager::PlayAmbient(std::string name)
{
    StopAmbient();

    ambientTrackName = name;
    std::string trackPath = "Assets/Sounds/ambient/"+name+".bgm";
    ambientTrack = oslLoadSoundFile(trackPath.c_str(),OSL_FMT_STREAM);

    oslPlaySound(ambientTrack,2);
}

void SoundManager::StopDiskTrack()
{
    if(diskTrack != NULL)
    {
        oslStopSound(diskTrack);
        oslDeleteSound(diskTrack);
        diskTrack = NULL;

        diskTrackName = " ";
    }
}

void SoundManager::PlayDiskTrack(int diskNumber)
{
	StopAmbient();
	StopDiskTrack();

	std::string name = " ";
	switch(diskNumber)
	{
	    case 0: name = "13"; break;
	    case 1: name = "cat"; break;
	    case 2: name = "blocks"; break;
	    case 3: name = "chirp"; break;
	    case 4: name = "far"; break;
	    case 5: name = "mall"; break;
	    case 6: name = "mellohi"; break;
	    case 7: name = "stal"; break;
	    case 8: name = "strad"; break;
	    case 9: name = "ward"; break;
	    case 10: name = "11"; break;
	    case 11: name = "wait"; break;
	}
    diskTrackName = name;
    std::string trackPath = "Assets/Sounds/disk/"+name+".bgm";
    diskTrack = oslLoadSoundFile(trackPath.c_str(),OSL_FMT_STREAM);

    oslPlaySound(diskTrack,2);
}

