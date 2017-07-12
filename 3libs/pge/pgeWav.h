/*
 * pgeWav.h: Header for WAV audio playback
 *
 * This file is part of "Phoenix Game Engine".
 *
 * Copyright (C) 2008 Phoenix Game Engine
 * Copyright (C) 2008 InsertWittyName <tias_dp@hotmail.com>
 * Copyright (C) 2008 MK2k <pge@mk2k.net>
 *
 * Phoenix Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Phoenix Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Phoenix Game Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PGEWAV_H__
#define __PGEWAV_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeWav WAV Library
 *  @{
 */

/**
 * A WAV file struct
 */
typedef struct
{
	unsigned int samplerate;	/**<  Sample rate */
	unsigned int samplecount;	/**<  Sample count */
	unsigned int datalength;	/**<  Data length */
	unsigned int rateratio;		/**<  Rate ratio (samplerate / 44100 * 0x10000) */
	unsigned int playptr;		/**<  Current play position */
	unsigned int playptrfrac;	/**<  Current fractional play position */
	unsigned int loop;			/**<  Loop flag */
	unsigned int id;			/**<  The ID of the WAV */
	short channels;				/**<  Number of channels */
	short bits;					/**<  The bit size of the WAV */
	float leftvolume;			/**<  The left volume of the WAV */
	float rightvolume;			/**<  The right volume of the WAV */
	unsigned char *data;		/**<  A pointer to the actual WAV data */
		
} pgeWav;

/**
 * Initialise the WAV playback
 *
 * @returns 1 on success.
 */
int pgeWavInit(void);

/**
 * Shutdown WAV playback
 */
void pgeWavShutdown(void);

/**
 * Load a WAV file
 *
 * @param filename - Path of the file to load.
 *
 * @returns A pointer to a ::pgeWav struct or NULL on error.
 */
pgeWav *pgeWavLoad(const char *filename);

/**
 * Load a WAV file from memory
 *
 * @param buffer - Buffer that contains the WAV data.
 *
 * @param size - Size of the buffer.
 *
 * @returns A pointer to a ::pgeWav struct or NULL on error.
 */
pgeWav *pgeWavLoadMemory(unsigned char *buffer, int size);

/**
 * Destroy a previously loaded WAV file
 *
 * @param wav - A valid ::pgeWav
 */
void pgeWavDestroy(pgeWav *wav);

/**
 * Start playing a loaded WAV file
 *
 * @param wav A pointer to a valid ::pgeWav struct.
 *
 * @returns 1 on success.
 */
int pgeWavPlay(pgeWav *wav);

/**
 * Stop playing a loaded WAV
 *
 * @param wav A pointer to a valid ::pgeWav struct.
 *
 * @returns 1 on success.
 */
void pgeWavStop(pgeWav *wav);

/**
 * Stop playing all WAVs
 */
void pgeWavStopAll(void);

/**
 * Check if a WAV is playing
 *
 * @param wav A pointer to a valid ::pgeWav struct.
 *
 * @returns 1 if playing, 0 if not.
 */
int pgeWavIsPlaying(pgeWav *wav);

/**
 * Set the loop of the WAV
 *
 * @param wav - A pointer to a valid ::pgeWav struct.
 *
 * @param loop - Set to 1 to loop, 0 to playback once.
 */
void pgeWavLoop(pgeWav *wav, unsigned int loop);

/**
 * Set the volume of the WAV
 *
 * @param wav - A pointer to a valid ::pgeWav struct.
 *
 * @param leftvolume - Set to 0 for no volume, 100 for full volume.
 *
 * @param rightvolume - Set to 0 for no volume, 100 for full volume.
 */
void pgeWavVolume(pgeWav *wav, unsigned char leftvolume, unsigned char rightvolume);

/**
 * Set the pitch of the WAV
 *
 * @param wav - A pointer to a valid ::pgeWav struct.
 *
 * @param pitch - The new pitch.
 */
void pgeWavPitch(pgeWav *wav, float pitch);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEWAV_H__
