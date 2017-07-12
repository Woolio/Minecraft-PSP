#ifndef CHUNKDATAPROVIDER_H_
#define CHUNKDATAPROVIDER_H_

#include <pspkerneltypes.h>

#include <sys/time.h>
#include <Aurora/Utils/Logger.h>
#include <Aurora/Utils/Timer.h>

#include <fstream>
#include <ostream>

#include <algorithm>

class ChunkDataProvider
{

protected:
    static int ChunkDataDealer(SceSize args, void *argp); //static int
	SceUID thid_2;

public:
	ChunkDataProvider();
	void KillDealer();
	static bool writeChunk;
	static bool busy;
	static int so;
};

#endif
