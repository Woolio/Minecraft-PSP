#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

extern "C"
{
    #include "sound_utils/pgeWav.h"
    #include "sound_utils/oslib.h"
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Aurora/Utils/Logger.h>

#include<fstream>
#include<iostream>


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager m_SoundManager;
	static SoundManager* Instance();

	void Init();
	void AudioSync();

    void PlayBreakSound();
    void PlayHitSound();
    void PlayEatSound();

    void PlayZombieHurtSound(float distanceToPlayer);
    void PlayZombieDieSound(float distanceToPlayer);
    void PlayZombieSaySound(float distanceToPlayer);

    void PlayCowHurtSound(float distanceToPlayer);
    void PlayCowSaySound(float distanceToPlayer);

    void PlayCreeperHurtSound(float distanceToPlayer);
    void PlaySheepHurtSound(float distanceToPlayer);

    void PlayRainSound(int volume);

    void PlayFissSound();
    void PlaySplashSound();
    void PlayFuseSound();
    void PlayShearSound();
    void PlayBowSound();
	void PlayMenuSound();
	void PlayPlopSound();
	void PlayWalkSound(int type);
	void PlayDigSound(int type);
	void PlayEndDigSound(int type);
	void TNTSound();
	void PlayDoorSound(bool open);
	void PlayFallSound(float i);
	void PlayGlassSound();
    void PlayNoteSound(unsigned int noteType, float pitch);

    void PlayDiskTrack(int diskNumber);
    void StopDiskTrack();

	int PlayRandomAmbient();

	void StopAmbient();
	void PlayAmbient(std::string name);

	bool playerSounds;
	bool ambientSoundsEnabled;

	int currentAmbientSound;
	int lastAmbientSound;

	bool delayAmbient;

private:

	int currentWalkSound;
	int lastWalkSound;

	//sounds
	pgeWav *buttonSound;
	//plop sound
	pgeWav *plopSound;
	//grass sounds
	pgeWav *grassSounds[4];
	pgeWav *grawelSounds[4];
	pgeWav *stoneSounds[4];
	pgeWav *woodSounds[4];
	pgeWav *sandSounds[4];
	pgeWav *snowSounds[4];

	pgeWav *eatSounds[3];

	pgeWav *hitSounds[3];
    pgeWav *fallSounds[2];

    pgeWav *zombieHurtSounds[2];
    pgeWav *zombieSaySound;
    pgeWav *zombieDieSound;

    pgeWav *cowHurtSounds[2];
    pgeWav *cowSaySound;

    pgeWav *creeperHurtSounds[2];

    pgeWav *sheepHurtSounds[2];

	pgeWav *doorOpen;
	pgeWav *doorClose;
	pgeWav *tnt;
	pgeWav *bow;
	pgeWav *breakSound;
	pgeWav *fuse;
	pgeWav *fiss;
	pgeWav *splash;
	pgeWav *glass;
	pgeWav *shear;

	pgeWav *rainSound;

	pgeWav *digSounds[3];

	pgeWav *noteSounds[2]; // bass and pling

	// this lib can stream
	OSL_SOUND *ambientTrack;
	std::string ambientTrackName;

	OSL_SOUND *diskTrack;
	std::string diskTrackName;

};

#endif

