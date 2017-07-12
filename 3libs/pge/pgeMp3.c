/*
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

#include <pspkernel.h>
#include <pspsdk.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pspaudiocodec.h>
#include <pspaudio.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pspdisplay.h>
#include <psppower.h>

#include "pgeMp3.h"
#include "pgeFile.h"
#include "pge.h"

#define PGE_MP3_MIN_INFRAME_SIZE (96)
#define PGE_MP3_READ_SIZE (2*1024)
#define PGE_MP3_IN_BUFFER_SIZE (1152 * 64)

#define PGE_MP3_BUFFER1_EMPTY	(0x00000001)
#define PGE_MP3_BUFFER2_EMPTY	(0x00000010)
#define PGE_MP3_BUFFER3_EMPTY	(0x00000100)
#define PGE_MP3_RESUMED			(0x00001000)
#define PGE_MP3_SUSPENDING		(0x00001000)

static int pgeMp3LastError = 0;
static int pgeMp3Initialized = 0;
static int pgeMp3ThreadExit = 0;
static int pgeMp3Looping = 0;
static int pgeMp3JobStarted = 0;
static int pgeMp3VolumeValue = 0x8000;
static int pgeMp3SamplesReady = 0;
static int pgeMp3BufferOffset = 0;
static int pgeMp3PlayBufferSel = 0;
static int pgeMp3OutputIndex = 0; 
static int pgeMp3WorkingBuffer = 0;
static const char *pgeMp3Filename = NULL;
static pgeFile *pgeMp3Handle = NULL;
static int pgeMp3DataPos = 0;
static int pgeMp3DataEnd = 0;
static int pgeMp3DataStart = 0;
static int pgeMp3SampleRate = 0;
static int pgeMp3AudioChannel = -1;
static int pgeMp3Playing = 0;
static int pgeMp3Paused = 0;
static int pgeMp3DecodeThreadId = -1;
static int pgeMp3ReadThreadId = -1;
static int pgeMp3BufferSize[2];
static int bufferempty = 0;
static int pgeMp3NumFrames = 0;
static int pgeMp3DecodeCurrentFrame = 0;
static int pgeMp3SamplesPerFrame = 0;
static int pgeMp3ShouldExit = 0;
static int pgeMp3ReadCurrentBuffer = 0;
static int pgeMp3ReadNumFrames = 0;
static int pgeMp3DecodeReady = 0;
static pgeMp3Info pgeMp3Id3TagInfo;
static int pgeMp3EventFlag = -1;

static int samplerates[4][3] =
{
    {11025,	12000,	8000,},		//MPEG 2.5
    {0,		0,		0,},		//reserved
    {22050,	24000,	16000,},	//MPEG 2
    {44100,	48000,	32000}		//MPEG 1
};

static int bitrates[] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320};
static int bitrates_v2[] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160};

static unsigned long pgeMp3CodecStruct[65] __attribute__((aligned(64)));

static short pgeMp3SrcBuffer[3][PGE_MP3_IN_BUFFER_SIZE*2] __attribute__((aligned(64)));
static short pgeMp3OutputBuffer[4][1152*2] __attribute__((aligned(64))); 
static unsigned char pgeMp3ReadBuffer[PGE_MP3_READ_SIZE]__attribute__((aligned(64)));

typedef void *pgeMp3Id3Tag;

typedef struct
{
	int code;
	char *text;
	
} pgeMp3Genre;

static pgeMp3Genre pgeMp3GenreList[] = 
{
	{0 , "Blues"}, {1 , "Classic Rock"}, {2 , "Country"}, {3 , "Dance"}, {4 , "Disco"}, {5 , "Funk"}, {6 , "Grunge"}, {7 , "Hip-Hop"}, {8 , "Jazz"}, {9 , "Metal"}, {10 , "New Age"},
	{11 , "Oldies"}, {12 , "Other"}, {13 , "Pop"}, {14 , "R&B"}, {15 , "Rap"}, {16 , "Reggae"}, {17 , "Rock"}, {18 , "Techno"}, {19 , "Industrial"}, {20 , "Alternative"},
	{21 , "Ska"}, {22 , "Death Metal"}, {23 , "Pranks"}, {24 , "Soundtrack"}, {25 , "Euro-Techno"}, {26 , "Ambient"}, {27 , "Trip-Hop"}, {28 , "Vocal"}, {29 , "Jazz+Funk"}, {30 , "Fusion"},
	{31 , "Trance"}, {32 , "Classical"}, {33 , "Instrumental"}, {34 , "Acid"}, {35 , "House"}, {36 , "Game"}, {37 , "Sound Clip"}, {38 , "Gospel"}, {39 , "Noise"}, {40 , "Alternative Rock"},
	{41 , "Bass"}, {42 , "Soul"}, {43 , "Punk"}, {44 , "Space"}, {45 , "Meditative"}, {46 , "Instrumental Pop"}, {47 , "Instrumental Rock"}, {48 , "Ethnic"}, {49 , "Gothic"}, {50 , "Darkwave"},
	{51 , "Techno-Industrial"}, {52 , "Electronic"}, {53 , "Pop-Folk"}, {54 , "Eurodance"}, {55 , "Dream"}, {56 , "Southern Rock"}, {57 , "Comedy"}, {58 , "Cult"}, {59 , "Gangsta"}, {60 , "Top 40"},
	{61 , "Christian Rap"}, {62 , "Pop/Funk"}, {63 , "Jungle"}, {64 , "Native US"}, {65 , "Cabaret"}, {66 , "New Wave"}, {67 , "Psychadelic"}, {68 , "Rave"}, {69 , "Showtunes"}, {70 , "Trailer"},
	{71 , "Lo-Fi"}, {72 , "Tribal"}, {73 , "Acid Punk"}, {74 , "Acid Jazz"}, {75 , "Polka"}, {76 , "Retro"}, {77 , "Musical"}, {78 , "Rock & Roll"}, {79 , "Hard Rock"}, {80 , "Folk"},
	{81 , "Folk-Rock"}, {82 , "National Folk"}, {83 , "Swing"}, {84 , "Fast Fusion"}, {85 , "Bebob"}, {86 , "Latin"}, {87 , "Revival"}, {88 , "Celtic"}, {89 , "Bluegrass"}, {90 , "Avantgarde"},
	{91 , "Gothic Rock"}, {92 , "Progressive Rock"}, {93 , "Psychedelic Rock"}, {94 , "Symphonic Rock"}, {95 , "Slow Rock"}, {96 , "Big Band"}, {97 , "Chorus"}, {98 , "Easy Listening"}, {99 , "Acoustic"},
	{100 , "Humour"}, {101 , "Speech"}, {102 , "Chanson"}, {103 , "Opera"}, {104 , "Chamber Music"}, {105 , "Sonata"}, {106 , "Symphony"}, {107 , "Booty Bass"}, {108 , "Primus"}, {109 , "Porn Groove"},
	{110 , "Satire"}, {111 , "Slow Jam"}, {112 , "Club"}, {113 , "Tango"}, {114 , "Samba"}, {115 , "Folklore"}, {116 , "Ballad"}, {117 , "Power Ballad"}, {118 , "Rhytmic Soul"}, {119 , "freestyle"}, {120 , "Duet"},
	{121 , "Punk Rock"}, {122 , "Drum Solo"}, {123 , "Acapella"}, {124 , "Euro-House"}, {125 , "Dance Hall"}, {126 , "Goa"}, {127 , "Drum & Bass"}, {128 , "Club-House"}, {129 , "Hardcore"}, {130 , "Terror"},
	{131 , "Indie"}, {132 , "BritPop"}, {133 , "Negerpunk"}, {134 , "Polsk Punk"}, {135 , "Beat"}, {136 , "Christian Gangsta"}, {137 , "Heavy Metal"}, {138 , "Black Metal"}, {139 , "Crossover"}, {140 , "Contemporary C"},
	{141 , "Christian Rock"}, {142 , "Merengue"}, {143 , "Salsa"}, {144 , "Thrash Metal"}, {145 , "Anime"}, {146 , "JPop"}, {147 , "SynthPop"}
};

static int pgeMp3GenreCount = sizeof(pgeMp3GenreList) / sizeof(pgeMp3Genre);

static int pgeMp3SeekPastNull(pgeFile *file)
{
	char buffer;
	
	int i = 0;
	
	while(pgeRunning())
	{
		pgeFileRead(file, &buffer, 1);
		i++;
		
		if(buffer == '\0')
			break;
	}
	
	return i;
}	

static short swapInt16BigToHost(short arg)
{
   short i = 0;
   int checkendian = 1;
   
   if(1 == *(char *)&checkendian)
   {
      // Little endian
      i = arg;
      i = ((i&0xFF00)>>8)|((i&0x00FF)<<8);
   }
   else
   {
      // Big endian
      i = arg;
   }
   return i;
}

static int swapInt32BigToHost(int arg)
{
   int i = 0;
   int checkendian = 1;
   
   if(1 == *(char *)&checkendian)
   {
      // Intel (little endian)
      i = arg;
      i = ((i&0xFF000000)>>24)|((i&0x00FF0000)>>8)|((i&0x0000FF00)<<8)|((i&0x000000FF)<<24);
   }
   else
   {
      // PPC (big endian)
      i = arg;
   }
   return i;
}

static int ID3v2TagSize(void)
{
	int size;
	char sig[3];
	
	pgeFileSeek(pgeMp3Handle, 0, PGE_FILE_SET);
	pgeFileRead(pgeMp3Handle, sig, 3);
	
	if(strncmp("ID3", sig, 3) != 0)
		return 0;

	pgeFileSeek(pgeMp3Handle, 6, PGE_FILE_SET);
	
	pgeFileRead(pgeMp3Handle, &size, 4);
	
	size = (unsigned int) swapInt32BigToHost((int)size);
	size = ( ( (size & 0x7f000000) >> 3 ) | ( (size & 0x7f0000) >> 2 ) | ( (size & 0x7f00) >> 1 ) | (size & 0x7f) );
	
	return size;
}

static int ID3v2(void)
{
	char sig[3];
	unsigned short version;
	
	pgeFileSeek(pgeMp3Handle, 0, PGE_FILE_SET);
	
	pgeFileRead(pgeMp3Handle, sig, 3);
	
	if(!strncmp("ID3", sig, 3))
	{
		pgeFileRead(pgeMp3Handle, &version, 2);
		version = (unsigned short int) swapInt16BigToHost((short int)version);
		version /= 256;
	}
	
	return (int)version;
}

static void ParseID3v2_2(pgeMp3Info *id3tag)
{
	int size;
	int tag_length;
	char tag[3];
	int skip;
	
	if(ID3v2() == 2)
	{
		size = ID3v2TagSize();
		
		pgeFileSeek(pgeMp3Handle, 10, PGE_FILE_SET);
		
		while(size != 0)
		{
			pgeFileRead(pgeMp3Handle, tag, 3);
			size -= 3;
			
			pgeFileRead(pgeMp3Handle, &tag_length, 4);
			
			pgeFileSeek(pgeMp3Handle, -1, PGE_FILE_CUR);
			
			tag_length = (unsigned int) swapInt32BigToHost((int)tag_length);
			tag_length = (tag_length / 256);
			size -= 3;
			
			if(*tag == 0 || tag_length > size || tag_length == 0)
				break;
			
			if(!strncmp("TP1", tag, 3)) /* Artist */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->artist = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->artist, tag_length - 1);
				id3tag->artist[tag_length - 1] = '\0';
			}
			else if(!strncmp("TP2", tag, 3)) /* Title */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->title = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->title, tag_length - 1);
				id3tag->title[tag_length - 1] = '\0';
			}
			else if(!strncmp("TAL", tag, 3)) /* Album */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->album = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->album, tag_length - 1);
				id3tag->album[tag_length - 1] = '\0';
			}
			else if(!strncmp("TRK", tag, 3)) /* Track No. */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->tracktext = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->tracktext, tag_length - 1);
				id3tag->tracktext[tag_length - 1] = '\0';
				id3tag->track = atoi(id3tag->tracktext);
			}
			else if(!strncmp("TYE", tag, 3)) /* Year */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->year = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->year, tag_length - 1);
				id3tag->year[tag_length - 1] = '\0';
			}
			else if(!strncmp("COM", tag, 3))
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				skip = pgeMp3SeekPastNull(pgeMp3Handle);
				id3tag->comment = (char *)pgeMalloc(tag_length - skip);
				pgeFileRead(pgeMp3Handle, id3tag->comment, tag_length - skip - 1);
				id3tag->comment[tag_length - skip - 1] = '\0';
			}
			else if(!strncmp("TCO", tag, 3)) /* Genre */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->genretext = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->genretext, tag_length - 1);
				id3tag->genretext[tag_length - 1] = '\0';
			}
			else if(!strncmp("PIC", tag, 3)) /* Picture */
			{
				pgeFileSeek(pgeMp3Handle, 6, PGE_FILE_CUR);
				id3tag->pictureoffset = pgeFileTell(pgeMp3Handle);
				pgeFileSeek(pgeMp3Handle, tag_length - 6, PGE_FILE_CUR);
			}
			else
			{
				pgeFileSeek(pgeMp3Handle, tag_length, PGE_FILE_CUR);
			}
			
			size -= tag_length;
		}
		
		strcpy(id3tag->id3version, "2.2");
	}
}

static void ParseID3v2_3(pgeMp3Info *id3tag)
{
	int size;
	int tag_length = 0;
	char tag[4];
	int skip;
	
	if(ID3v2() == 3)
	{
		size = ID3v2TagSize();

		pgeFileSeek(pgeMp3Handle, 10, PGE_FILE_SET);
		
		while(size > 0)
		{
			pgeFileRead(pgeMp3Handle, tag, 4);
			size -= 4;
			
			pgeFileRead(pgeMp3Handle, &tag_length, 4);
			tag_length = (unsigned int) swapInt32BigToHost((int)tag_length);
			size -= 4;
			
			pgeFileSeek(pgeMp3Handle, 2, PGE_FILE_CUR);
			size -= 2;
			
			if(*tag == 0 || tag_length > size || tag_length == 0)
				break;
				
			if(!strncmp("TPE1", tag, 4)) /* Artist */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->artist = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->artist, tag_length - 1);
				id3tag->artist[tag_length - 1] = '\0';
			}
			else if(!strncmp("TIT2", tag, 4)) /* Title */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->title = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->title, tag_length - 1);
				id3tag->title[tag_length - 1] = '\0';
			}
			else if(!strncmp("TALB", tag, 4)) /* Album */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->album = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->album, tag_length - 1);
				id3tag->album[tag_length - 1] = '\0';
			}
			else if(!strncmp("TRCK", tag, 4)) /* Track No. */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->tracktext = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->tracktext, tag_length - 1);
				id3tag->tracktext[tag_length - 1] = '\0';
				id3tag->track = atoi(id3tag->tracktext);
			}
			else if(!strncmp("TYER", tag, 4)) /* Year */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->year = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->year, tag_length - 1);
				id3tag->year[tag_length - 1] = '\0';
			}
			else if(!strncmp("COMM", tag, 4)) /* Comment */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				skip = pgeMp3SeekPastNull(pgeMp3Handle);
				id3tag->comment = (char *)pgeMalloc(tag_length - skip);
				pgeFileRead(pgeMp3Handle, id3tag->comment, tag_length - skip - 1);
				id3tag->comment[tag_length - skip - 1] = '\0';
			}
			else if(!strncmp("TCON", tag, 4)) /* Genre */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->genretext = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->genretext, tag_length - 1);
				id3tag->genretext[tag_length - 1] = '\0';
			}
			else if(!strncmp("APIC", tag, 4)) /* Picture */
			{
				pgeFileSeek(pgeMp3Handle, 13, PGE_FILE_CUR);
				id3tag->pictureoffset = pgeFileTell(pgeMp3Handle);
				pgeFileSeek(pgeMp3Handle, tag_length - 13, PGE_FILE_CUR);
			}
			else
			{
				pgeFileSeek(pgeMp3Handle, tag_length, PGE_FILE_CUR);
			}
			
			size -= tag_length;
		}
		
		strcpy(id3tag->id3version, "2.3");
	}
}

static void ParseID3v2_4(pgeMp3Info *id3tag)
{
	int size;
	int tag_length;
	char tag[4];
	int skip;
	
	if(ID3v2() == 4)
	{
		size = ID3v2TagSize();

		pgeFileSeek(pgeMp3Handle, 10, PGE_FILE_SET);
		
		while(size != 0)
		{
			pgeFileRead(pgeMp3Handle, tag, 4);
			size -= 4;
			
			pgeFileRead(pgeMp3Handle, &tag_length, 4);
			tag_length = (unsigned int) swapInt32BigToHost((int)tag_length);
			size -= 4;
			
			pgeFileSeek(pgeMp3Handle, 2, PGE_FILE_CUR);
			size -= 2;
			
			if(*tag == 0 || tag_length > size || tag_length == 0)
				break;
				
			if(!strncmp("TPE1", tag, 4)) /* Artist */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->artist = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->artist, tag_length - 1);
				id3tag->artist[tag_length - 1] = '\0';
			}
			else if(!strncmp("TIT2", tag, 4)) /* Title */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->title = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->title, tag_length - 1);
				id3tag->title[tag_length - 1] = '\0';
			}
			else if(!strncmp("TALB", tag, 4)) /* Album */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->album = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->album, tag_length - 1);
				id3tag->album[tag_length - 1] = '\0';
			}
			else if(!strncmp("TRCK", tag, 4)) /* Track No. */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->tracktext = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->tracktext, tag_length - 1);
				id3tag->tracktext[tag_length - 1] = '\0';
				id3tag->track = atoi(id3tag->tracktext);
			}
			else if(!strncmp("TYER", tag, 4)) /* Year */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->year = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->year, tag_length - 1);
				id3tag->year[tag_length - 1] = '\0';
			}
			else if(!strncmp("COMM", tag, 4))
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				skip = pgeMp3SeekPastNull(pgeMp3Handle);
				id3tag->comment = (char *)pgeMalloc(tag_length - skip);
				pgeFileRead(pgeMp3Handle, id3tag->comment, tag_length - skip - 1);
				id3tag->comment[tag_length - skip - 1] = '\0';
			}
			else if(!strncmp("TCON", tag, 4)) /* Genre */
			{
				pgeFileSeek(pgeMp3Handle, 1, PGE_FILE_CUR);
				id3tag->genretext = (char *)pgeMalloc(tag_length);
				pgeFileRead(pgeMp3Handle, id3tag->genretext, tag_length - 1);
				id3tag->genretext[tag_length - 1] = '\0';
			}
			else if(!strncmp("APIC", tag, 4)) /* Picture */
			{
				pgeFileSeek(pgeMp3Handle, 13, PGE_FILE_CUR);
				id3tag->pictureoffset = pgeFileTell(pgeMp3Handle);
				pgeFileSeek(pgeMp3Handle, tag_length - 13, PGE_FILE_CUR);
			}
			else
			{
				pgeFileSeek(pgeMp3Handle, tag_length, PGE_FILE_CUR);
			}
			
			size -= tag_length;
		}
		
		strcpy(id3tag->id3version, "2.4");
	}
}

static int ParseID3v2(pgeMp3Info *id3tag)
{
	switch(ID3v2())
	{
		case 2:
			ParseID3v2_2(id3tag);
			break;
			
		case 3:
			ParseID3v2_3(id3tag);
			break;
			
		case 4:
			ParseID3v2_4(id3tag);
			break;
			
		default:
			return -1;
	}
	
	return 0;
}

static int ParseID3v1(pgeMp3Info *id3tag)
{
    char id3buffer[512];
		
    pgeFileSeek(pgeMp3Handle, -128, PGE_FILE_END);
    pgeFileRead(pgeMp3Handle, id3buffer, 128);

    if(strstr(id3buffer, "TAG") != NULL)
	{
		if(id3tag->title == NULL)
		{
			pgeFileSeek(pgeMp3Handle, -125, PGE_FILE_END);
			id3tag->title = (char *)pgeMalloc(31);
			pgeFileRead(pgeMp3Handle, id3tag->title, 30);
			id3tag->title[30] = '\0';
		}
    
        if(id3tag->artist == NULL)
		{
			pgeFileSeek(pgeMp3Handle, -95, PGE_FILE_END);
			id3tag->artist = (char *)pgeMalloc(31);
			pgeFileRead(pgeMp3Handle, id3tag->artist, 30);
			id3tag->artist[30] = '\0';
		}
		
		if(id3tag->album == NULL)
		{
			pgeFileSeek(pgeMp3Handle, -65, PGE_FILE_END);
			id3tag->album = (char *)pgeMalloc(31);
			pgeFileRead(pgeMp3Handle, id3tag->album, 30);
			id3tag->album[30] = '\0';
		}
    
        if(id3tag->year == NULL)
		{
			pgeFileSeek(pgeMp3Handle, -35, PGE_FILE_END);
			id3tag->year = (char *)pgeMalloc(5);
			pgeFileRead(pgeMp3Handle, id3tag->year, 4);
			id3tag->year[4] = '\0';
		}
    
        if(id3tag->comment == NULL)
		{
			pgeFileSeek(pgeMp3Handle, -31, PGE_FILE_END);
			id3tag->comment = (char *)pgeMalloc(31);
			pgeFileRead(pgeMp3Handle, id3tag->comment, 30);
			id3tag->comment[30] = '\0';
		}
    
        if(id3tag->genrecode == NULL)
		{
			pgeFileSeek(pgeMp3Handle, -1, PGE_FILE_END);
			id3tag->genrecode = (char *)pgeMalloc(2);
			pgeFileRead(pgeMp3Handle, id3tag->genrecode, 1);
			id3tag->genrecode[1] = '\0';
		}

        /* Track */
        if(*(id3tag->comment + 28) == 0 && *(id3tag->comment + 29) > 0)
		{
			id3tag->track = *(id3tag->comment + 29);
			strcpy(id3tag->id3version, "1.1");
        }
		else
		{
			id3tag->track = 0;
			strcpy(id3tag->id3version, "1.0");
		}
		
		if(id3tag->genretext == NULL)
		{
			id3tag->genretext = (char *)pgeMalloc(31);
			
			if(((int)id3tag->genrecode[0] >= 0) & ((int)id3tag->genrecode[0] < pgeMp3GenreCount))
				strcpy(id3tag->genretext, pgeMp3GenreList[(int)id3tag->genrecode[0]].text);
			else
				strcpy(id3tag->genretext, "");
		
			id3tag->genretext[30] = '\0';
		}
		
        return 0;
	}
	
	return -1;
}

// Main function:
static void ParseID3(pgeMp3Info *id3tag)
{
	memset(id3tag, 0, sizeof(pgeMp3Info));
    
    ParseID3v2(id3tag);
	ParseID3v1(id3tag);
}

///////////////////////
// INTERNAL FUNCTIONS
///////////////////////

static int pgeMpCheckId3TagV2(pgeFile *uid)
{	
	char identifier[3];
	
	pgeFileSeek(uid, 0, PGE_FILE_SET);
	pgeFileRead(uid, identifier, 3);
	
	if(strncmp(identifier, "ID3", 3) != 0)
		return 0;
	
	unsigned char majorVersion;
	
	pgeFileRead(uid, &majorVersion, 1);
	
	if (majorVersion < 2 || majorVersion > 4)
		return -1;
	
	unsigned char flags;

	pgeFileSeek(uid, 1, PGE_FILE_CUR);
	
	pgeFileRead(uid, &flags, 1);
	
	switch(majorVersion)
	{
		case 2:
			if(flags)
				return -1;
			
			break;
		
		case 3:
			if(flags & ~0x40) // Extended header
				return -1;
			
			break;
		
		case 4:
			if(flags & ~(0x40 | 0x10)) // Extended header or footer
				return -1;
			
			break;
	}
	
	unsigned int tagSize;
	
	pgeFileRead(uid, &tagSize, 4);
	
	unsigned int nTagSize = swapInt32BigToHost(tagSize);
	
	nTagSize = (nTagSize & 0x0000007f)
		| ((nTagSize & 0x00007f00) >> 1)
		| ((nTagSize & 0x007f0000) >> 2)
		| ((nTagSize & 0x7f000000) >> 3);
	nTagSize += 10;
	
	return nTagSize;
}

static int pgeMpCheckId3TagV1(pgeFile *uid)
{
	char identifier[3];
	
	pgeFileSeek(uid, -128, PGE_FILE_END);
	
	pgeFileRead(uid, identifier, 3);
	
	if(strncmp(identifier, "TAG", 3) != 0)
		return 0;
		
	return 1;
}

static int pgeMp3FindSyncWord(unsigned char *data, int len)
{
	int i;
	
	for(i = 0; i < len-1;i++)
	{
		if(data[i+0] != 0xff)
			continue;
			
		if((data[i+1] & 0xE0) == 0xE0)
			break;
			
		i++;
	}
		
	return i;
}

static int pgeMp3ReadNextFrame(int buffer)
{	
	int i, frameoffset;
	int bitrate, padding, version, framesize = 0;
	int required = PGE_MP3_READ_SIZE;
	u32 outbits;
	
	pgeFileSeek(pgeMp3Handle, pgeMp3DataPos, PGE_FILE_SET);

	for(i = 0; i < 64; i++)
	{
		if(pgeMp3ThreadExit)
			return 0;
		
		sceKernelPollEventFlag(pgeMp3EventFlag, PGE_MP3_RESUMED, PSP_EVENT_WAITAND, &outbits);
			
		if(outbits & PGE_MP3_RESUMED)
		{
			pgeFileClose(pgeMp3Handle);
			
			char dir[256];
			getcwd(dir, 256);
			sceIoChdir(dir);

			pgeMp3Handle = pgeFileOpen((char*)pgeMp3Filename, PGE_FILE_RDONLY);
		
			pgeFileSeek(pgeMp3Handle, pgeMp3DataPos, PGE_FILE_SET);

			sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_RESUMED);
		}
		
		if(pgeMp3Handle == NULL)
		{
			sceKernelSetEventFlag(pgeMp3EventFlag, PGE_MP3_RESUMED);
			
			return 0;
		}

		pgeFileSeek(pgeMp3Handle, -2, PGE_FILE_CUR);
			
		required = PGE_MP3_READ_SIZE;
			
		if(pgeMp3DataPos + required > pgeMp3DataEnd)
		{
			required = (pgeMp3DataEnd - pgeMp3DataPos);
			pgeMp3ShouldExit = 1;
		}
		
		pgeFileRead(pgeMp3Handle, pgeMp3ReadBuffer, required);

		pgeMp3DataPos += required;
		pgeMp3BufferSize[pgeMp3ReadCurrentBuffer] = required;
		
		frameoffset = pgeMp3FindSyncWord(pgeMp3ReadBuffer, required);
	
		version = (pgeMp3ReadBuffer[frameoffset+1] >> 3) & 3;
		
		pgeMp3SampleRate = samplerates[version][(pgeMp3ReadBuffer[frameoffset+2] >> 2) & 3];
		bitrate = pgeMp3ReadBuffer[frameoffset+2] >> 4;
		padding = (pgeMp3ReadBuffer[frameoffset+2] & 2) >> 1;
		
		if(version == 3)
		{
			pgeMp3SamplesPerFrame = 1152;
			framesize = 144000*bitrates[bitrate]/pgeMp3SampleRate + padding;
		}
		else
		{
			pgeMp3SamplesPerFrame = 576;
			framesize = 72000*bitrates_v2[bitrate]/pgeMp3SampleRate + padding;
		}
			
		pgeMp3CodecStruct[6] = (unsigned long)&pgeMp3ReadBuffer[frameoffset];
		pgeMp3CodecStruct[8] = (unsigned long)&pgeMp3SrcBuffer[buffer][i*pgeMp3SamplesPerFrame*2];
		pgeMp3CodecStruct[7] = pgeMp3CodecStruct[10] = framesize;
		pgeMp3CodecStruct[9] = pgeMp3SamplesPerFrame * 4;

		int ret = sceAudiocodecDecode(pgeMp3CodecStruct, 0x1002);
				
		if(ret < 0)
		{
			printf("sceAudiocodecDecode failed with %08x\n", ret);
		}

		pgeMp3DataPos -= required - (frameoffset + framesize);
		pgeFileSeek(pgeMp3Handle, pgeMp3DataPos, PGE_FILE_SET);
		
		pgeMp3ReadNumFrames++;
		
		if(pgeMp3ShouldExit)
		{
			if(pgeMp3Looping)
			{
				pgeMp3DataPos = pgeMp3DataStart;
				pgeFileSeek(pgeMp3Handle, pgeMp3DataPos, PGE_FILE_SET);
				pgeMp3ShouldExit = 0;
			}
			else
			{
				pgeMp3NumFrames = pgeMp3ReadNumFrames;
				return 0;
			}
		}
	}

	return 1;
}

static void pgeMp3InitCleanup(int level, int ret)
{
	switch(level)
	{
		case 3:
		case 2:
		case 1:
			sceAudiocodecReleaseEDRAM(pgeMp3CodecStruct);
		case 0:
			pgeMp3LastError = ret;
			pgeMp3Initialized = 0;
			break;
		
		default:
			break;
	}
}

static int pgeMp3DecodeThread(SceSize args, void *argp)
{	
	(void)args;
	(void)argp;
	
	pgeMp3AudioChannel = sceAudioSRCChReserve(pgeMp3SamplesPerFrame, pgeMp3SampleRate, 2);
	
	pgeMp3WorkingBuffer = 0;
	
	u32 outbits;

	while(!pgeMp3ThreadExit)
	{
		if(pgeMp3ThreadExit)
			break;

		int numframes = 0;
		
		while(pgeMp3JobStarted)
		{
			sceKernelDelayThreadCB(1000);
			
			if(pgeMp3ThreadExit)
				break;
				
			if(!pgeMp3Paused)
			{
				sceKernelPollEventFlag(pgeMp3EventFlag, PGE_MP3_BUFFER1_EMPTY, PSP_EVENT_WAITAND, &outbits);
										
				if(((outbits & PGE_MP3_BUFFER1_EMPTY) == 0 && pgeMp3WorkingBuffer == 0) || ((outbits & PGE_MP3_BUFFER2_EMPTY) == 0 && pgeMp3WorkingBuffer == 1) || ((outbits & PGE_MP3_BUFFER3_EMPTY) == 0 && pgeMp3WorkingBuffer == 2))
					sceKernelDelayThreadCB(1);
				else
					continue;
						
				sceAudioSRCOutputBlocking(pgeMp3VolumeValue, &pgeMp3SrcBuffer[pgeMp3WorkingBuffer][pgeMp3DecodeCurrentFrame*pgeMp3SamplesPerFrame*2]);
					
				pgeMp3DecodeCurrentFrame++;
				numframes++;
					
				if(pgeMp3DecodeCurrentFrame >= 64)
				{
					if(pgeMp3WorkingBuffer == 0)
						sceKernelSetEventFlag(pgeMp3EventFlag, PGE_MP3_BUFFER1_EMPTY);
					else if(pgeMp3WorkingBuffer == 1)
						sceKernelSetEventFlag(pgeMp3EventFlag, PGE_MP3_BUFFER2_EMPTY);
					else if(pgeMp3WorkingBuffer == 2)
						sceKernelSetEventFlag(pgeMp3EventFlag, PGE_MP3_BUFFER3_EMPTY);
						
					pgeMp3WorkingBuffer = (pgeMp3WorkingBuffer+1)%3;
					pgeMp3DecodeCurrentFrame = 0;
					bufferempty = 1;
				}
				
				//printf("numframes: %d\n", numframes);
				//printf("pgeMp3NumFrames: %d\n", pgeMp3NumFrames);
					
				if(numframes == pgeMp3NumFrames)
				{
					if(!pgeMp3Looping)
					{
						printf("Here");
						pgeMp3Stop();
						pgeMp3NumFrames = 0;
						break;
					}
				}
			}
			else
			{
				memset(pgeMp3OutputBuffer[pgeMp3OutputIndex], 0, sizeof(pgeMp3OutputBuffer[pgeMp3OutputIndex]));
				sceKernelDelayThreadCB(1000);
			}
		}

		if(!pgeMp3JobStarted)
		{
			if(pgeMp3Handle != NULL)
				pgeFileClose(pgeMp3Handle);
				
			pgeMp3Handle = NULL;
			pgeMp3Filename = NULL;
		}
		
	}
	
	pgeMp3Playing = 0;
	
	sceKernelExitDeleteThread(0);
	
	return 0;
}

static int power_callback(int unknown, int pwrflags, void *common)
{	
	(void)unknown;
	(void)common;
	
	if((pwrflags & (PSP_POWER_CB_POWER_SWITCH|PSP_POWER_CB_STANDBY)) > 0)
	{
		pgeMp3Paused = 1;
	}
	
	if((pwrflags & 0x00040000) > 0)
	{
		sceKernelDelayThread(2*1000*1000);
		sceKernelSetEventFlag(pgeMp3EventFlag, PGE_MP3_RESUMED);
		pgeMp3Paused = 0;
	}
	
	return 0;
}

static int pgeMp3ReadThread(SceSize args, void *argp)
{		
	(void)args;
	(void)argp;
	
	int cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
	scePowerRegisterCallback(-1, cbid);
	
	pgeMp3ReadNextFrame(0);
	sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_BUFFER1_EMPTY);
	pgeMp3ReadNextFrame(1);
	sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_BUFFER2_EMPTY);
	pgeMp3ReadNextFrame(2);
	sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_BUFFER3_EMPTY);
	
	int result;
	u32 outbits;
	
	sceKernelStartThread(pgeMp3DecodeThreadId, 0, 0);
	
	while(!pgeMp3ThreadExit)
	{
		sceKernelDelayThreadCB(1000);
		
		sceKernelPollEventFlag(pgeMp3EventFlag, PGE_MP3_BUFFER1_EMPTY, PSP_EVENT_WAITAND, &outbits);
		
		if(outbits & PGE_MP3_BUFFER1_EMPTY)
		{
			result = pgeMp3ReadNextFrame(0);
			
			if(result)
				sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_BUFFER1_EMPTY);
		}
		
		if(outbits & PGE_MP3_BUFFER2_EMPTY)
		{
			result = pgeMp3ReadNextFrame(1);
			
			if(result)
				sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_BUFFER2_EMPTY);
		}
		
		if(outbits & PGE_MP3_BUFFER3_EMPTY)
		{
			result = pgeMp3ReadNextFrame(2);
			
			if(result)
				sceKernelClearEventFlag(pgeMp3EventFlag, ~PGE_MP3_BUFFER3_EMPTY);
		}
	}
	
	scePowerUnregisterCallback(cbid);
	
	sceKernelExitDeleteThread(0);
	
	return 0;
}

static int pgeMp3Init(void)
{
	int ret;
	
	pgeMp3ThreadExit = 0;

	pgeMp3LastError = 0;
	
	pgeMp3Initialized = 1;

	memset(pgeMp3CodecStruct, 0, sizeof(pgeMp3CodecStruct));
	
	ret = sceAudiocodecCheckNeedMem(pgeMp3CodecStruct, 0x1002);
	
	if(ret < 0)
	{
		printf("sceAudiocodecCheckNeedMem failed with %08x\n", ret);
		pgeMp3InitCleanup(0, ret);
	}

	ret = sceAudiocodecGetEDRAM(pgeMp3CodecStruct, 0x1002);
	
	if(ret < 0)
	{
		printf("sceAudiocodecGetEDRAM failed with %08x\n", ret);
		pgeMp3InitCleanup(0, ret);
	}

	ret = sceAudiocodecInit(pgeMp3CodecStruct, 0x1002);
	
	if(ret < 0)
	{
		printf("sceAudiocodecInit failed with %08x\n", ret);
		pgeMp3InitCleanup(1, ret);
	}
	
	pgeMp3ReadThreadId = sceKernelCreateThread("PgeMp3ReadThread", pgeMp3ReadThread, 14, 0x2000, PSP_THREAD_ATTR_USER, 0);
	
	if(pgeMp3ReadThreadId < 0)
	{
		printf("failed to create read thread: %08x\n", pgeMp3ReadThreadId);
		ret = pgeMp3ReadThreadId;
		pgeMp3InitCleanup(3, ret);
	}
	
	pgeMp3DecodeThreadId = sceKernelCreateThread("PgeMp3DecodeThread", pgeMp3DecodeThread, 15, 0x2000, PSP_THREAD_ATTR_USER, 0);
	
	if(pgeMp3DecodeThreadId < 0)
	{
		printf("failed to create decode thread: %08x\n", pgeMp3DecodeThreadId);
		ret = pgeMp3DecodeThreadId;
		pgeMp3InitCleanup(3, ret);
	}
	
	pgeMp3EventFlag = sceKernelCreateEventFlag("PgeMp3EventFlags", PSP_EVENT_WAITMULTIPLE, 0, NULL);
	
	if(pgeMp3EventFlag < 0)
	{
		printf("failed to create event flag: %08x\n", pgeMp3EventFlag);
		ret = pgeMp3EventFlag;
		pgeMp3InitCleanup(3, ret);
	}
	
	sceKernelSetEventFlag(pgeMp3EventFlag, PGE_MP3_BUFFER1_EMPTY|PGE_MP3_BUFFER2_EMPTY|PGE_MP3_BUFFER3_EMPTY);

	return 1;
}

static void pgeMp3Shutdown(void)
{
	pgeMp3Stop();
	pgeMp3Init();
}

///////////////////////
// EXTERNAL FUNCTIONS
///////////////////////

void pgeMp3Stop(void)
{
	if(!pgeMp3Initialized)
		return;
		
	pgeMp3ThreadExit = 1;
	
	sceKernelDelayThreadCB(100*1000);

	pgeFileClose(pgeMp3Handle);
		
	pgeMp3Handle = NULL;
	pgeMp3Filename = NULL;
	
	sceAudioSRCChRelease();
	sceAudiocodecReleaseEDRAM(pgeMp3CodecStruct);
	pgeMp3Playing = 0;
	pgeMp3Initialized = 0;

	pgeMp3DataStart = 0;
	pgeMp3DataPos = 0;
	pgeMp3DataEnd = 0;
	pgeMp3ShouldExit = 0;
	
	sceKernelDeleteEventFlag(pgeMp3EventFlag);
}

pgeMp3Info pgeMp3GetInfo(void)
{
	return pgeMp3Id3TagInfo;
}

static void freeMp3Info(pgeMp3Info *id3tag)
{
	if(id3tag->title)
		pgeFree(id3tag->title);
		
	if(id3tag->artist)
		pgeFree(id3tag->artist);
		
	if(id3tag->album)
		pgeFree(id3tag->album);
		
	if(id3tag->year)
		pgeFree(id3tag->year);
		
	if(id3tag->comment)
		pgeFree(id3tag->comment);
		
	if(id3tag->genrecode)
		pgeFree(id3tag->genrecode);
		
	if(id3tag->genretext)
		pgeFree(id3tag->genretext);
		
	if(id3tag->tracktext)
		pgeFree(id3tag->tracktext);
		
	id3tag->title = NULL;
	id3tag->artist = NULL;
	id3tag->album = NULL;
	id3tag->year = NULL;
	id3tag->comment = NULL;
	id3tag->genrecode = NULL;
	id3tag->genretext = NULL;
	id3tag->tracktext = NULL;
}

static int pgeMp3PlayInternal(void)
{	
	freeMp3Info(&pgeMp3Id3TagInfo);
	
	ParseID3(&pgeMp3Id3TagInfo);
	
	pgeFileSeek(pgeMp3Handle, 0, PGE_FILE_SET);
	
	int mp3ID3v2;
	int mp3ID3v1;
	
	pgeMp3Paused = 0;
	pgeMp3ThreadExit = 0;
	pgeMp3ReadCurrentBuffer = 0;
	pgeMp3ReadNumFrames = 0;
	pgeMp3DecodeCurrentFrame = 0;
	
	pgeMp3DecodeReady = 0;

	mp3ID3v2 = pgeMpCheckId3TagV2(pgeMp3Handle);
	mp3ID3v1 = pgeMpCheckId3TagV1(pgeMp3Handle);
	
	pgeMp3DataStart = pgeMp3DataPos = 0;
	
	if(mp3ID3v2 > 0)
		pgeMp3DataStart += mp3ID3v2;
		
	if(mp3ID3v1)
		pgeMp3DataEnd -= 128;
		
	pgeMp3DataPos = pgeMp3DataStart;
	
	pgeFileSeek(pgeMp3Handle, pgeMp3DataPos, PGE_FILE_SET);

	pgeMp3JobStarted = 1;
	pgeMp3SamplesReady = pgeMp3BufferOffset = pgeMp3PlayBufferSel = 0;
	pgeMp3WorkingBuffer = 0;
	
	bufferempty = 0;
	
	sceKernelStartThread(pgeMp3ReadThreadId, 0, 0);
	
	sceKernelDelayThreadCB(1); // Reschedule
	
	pgeMp3Playing = 1;
	
	return 1;
}

int pgeMp3Play(const char *filename)
{
	pgeMp3Shutdown();
	
	if(pgeMp3Handle == NULL)
	{
		pgeMp3Handle = pgeFileOpen((char*)filename, PGE_FILE_RDONLY);
		
		if(pgeMp3Handle == NULL)
		{
			pgeFileClose(pgeMp3Handle);
			return 0;
		}
		
		pgeMp3DataEnd = pgeFileSeek(pgeMp3Handle, 0, PGE_FILE_END);
		pgeMp3Filename = filename;
	}
	
	return pgeMp3PlayInternal();
}

int pgeMp3PlayMemory(unsigned char *data, unsigned int size)
{
	pgeMp3Shutdown();
	
	if(pgeMp3Handle == NULL)
	{
		pgeMp3Handle = pgeFileOpenMemory((char*)data, size);
		
		if(pgeMp3Handle == NULL)
		{
			pgeFileClose(pgeMp3Handle);
			return 0;
		}
		
		pgeMp3DataEnd = pgeFileSeek(pgeMp3Handle, 0, PGE_FILE_END);
		pgeMp3Filename = "memory";
	}
	
	return pgeMp3PlayInternal();
}

void pgeMp3Loop(int loop)
{
	pgeMp3Looping = loop;
}

void pgeMp3Pause(void)
{
	pgeMp3Paused ^= 1;
}

short *pgeMp3GetAudioData(void)
{
	return &pgeMp3SrcBuffer[pgeMp3WorkingBuffer][pgeMp3DecodeCurrentFrame*pgeMp3SamplesPerFrame*2];
}

int pgeMp3Volume(int volume)
{
	if(volume < 0)
		volume = 0;
	else if (volume > 100000)
		volume = 100000;
		
	pgeMp3VolumeValue = volume;
	
	return volume;
}

int pgeMp3IsPlaying(void)
{
	return pgeMp3Playing;
}
