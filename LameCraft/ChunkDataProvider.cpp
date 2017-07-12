#include "ChunkDataProvider.h"

#include <pspthreadman.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>

bool ChunkDataProvider::writeChunk = true;
bool ChunkDataProvider::busy = false;
int ChunkDataProvider::so = 0;

ChunkDataProvider::ChunkDataProvider()
{
	// the loadingscreen is loaded as a thread
	thid_2 = sceKernelCreateThread("ChunkHeart", ChunkDataDealer, 0x16, 0x10000, THREAD_ATTR_VFPU|THREAD_ATTR_USER, NULL);
	// start the thread
	sceKernelStartThread(thid_2, 0, 0);
}


void ChunkDataProvider::KillDealer()
{
	// shut down the dealer again.
	sceKernelTerminateDeleteThread(thid_2);
}


int ChunkDataProvider::ChunkDataDealer(SceSize args, void *argp)
{

    char *testChunkData;
    testChunkData = new  char[14 * 98 * 14];
    for(int i = 0; i < 14*98*14; i++)
    {
        testChunkData[i] = rand() % 122;
    }

    std::ofstream outfile ("test3.txt",std::ofstream::binary);

    outfile << "edsdfsergredsfgsdgfdfdsgsrsge";
    outfile.write (testChunkData,sizeof(char)*14*98*14);

    outfile.close();
    free(testChunkData);
    busy = false;

	return 0;
}

