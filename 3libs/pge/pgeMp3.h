/*
 * pgeMp3.h: Header for MP3
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

#ifndef __PGEMP3_H__
#define __PGEMP3_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeMp3 MP3 Library
 *  @{
 */
 
typedef struct
{
    char   *title;
    char   *artist;
    char   *album;
    char   *year;
    char   *comment;
    char   *genrecode;
    char   *genretext;
    char   id3version[4];
    int    track;    
    char   *tracktext;
    int    pictureoffset;
    int    picturelength;
    int    length;
	
} pgeMp3Info;

/**
 * Play an MP3 file
 *
 * @param filename - Path of the file to play.
 *
 * @returns 1 on success, 0 on error.
 */
int pgeMp3Play(const char *filename);

/**
 * Play an MP3 file (from memory)
 *
 * @param data - Pointer to the start of the MP3 data.
 *
 * @param size - Size of the data in bytes.
 *
 * @returns 1 on success, 0 on error.
 */
int pgeMp3PlayMemory(unsigned char *data, unsigned int size);

/**
 * Stop playing a loaded MP3 file
 */
void pgeMp3Stop(void);

/**
 * Pause a playing MP3 file
 *
 * @notes First call to this function will pause, second call will resume.
 */
void pgeMp3Pause(void);

/**
 * Set the loop status of the MP3 playback
 *
 * @param loop - Set to 1 to loop, 0 to playback once
 */
void pgeMp3Loop(int loop);

/**
 * Set the volume of the MP3 playback
 *
 * @param volume - The new volume. Accepted values 0 to 100000.
 *
 * @returns The volume set, between 0 to 100000.
 */
int pgeMp3Volume(int volume);

/**
 * Get the current output data of the MP3
 *
 * @returns A pointer to the current values in the output buffer.
 */
short *pgeMp3GetAudioData(void);

/**
 * Check if a MP3 is playing
 *
 * @returns 1 if an MP3 is playing, 0 if not.
 */
int pgeMp3IsPlaying(void);

/**
 * Get info about the playing MP3 (via ID3 tags)
 *
 * @returns A filled ::pgeMp3Info struct
 */
pgeMp3Info pgeMp3GetInfo(void);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEMP3_H__
