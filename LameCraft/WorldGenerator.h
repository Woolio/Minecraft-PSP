#ifndef WorldGenerator_H
#define WorldGenerator_H

#include <Aurora/Math/Frustum.h>
#include <math.h>
#include <psptypes.h>

#include <noisepp/Noise.h>
#include <noisepp/NoiseUtils.h>
#include <noisepp/NoiseBuilders.h>

#include <pspiofilemgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <zlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "SimplexNoise.h"
#include "Chest2.h"
#include "MonsterSpawnerEntity.h"



#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <psptypes.h>
#include <pspge.h>
#include <psputils.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <stdarg.h>

class CraftWorld;
using namespace noisepp;

class WorldGenerator
{
	public:
		void initRandompMap(int worldSize, int worldHeight, int chunkSize, CraftWorld *world, bool makeTrees,bool makeWater,bool makeCaves, unsigned int seedII, int worldType);
		void initTrees(int WORLD_SIZE,  CraftWorld *world, PerlinModule* perlin, int seed);
		void initRandomTrees(int WORLD_SIZE,  CraftWorld *world);

		void initPumpkins(int WORLD_SIZE,  CraftWorld *world);

		void initFlowers(int WORLD_SIZE, CraftWorld *world);
		void initGrass(int WORLD_SIZE, CraftWorld *world);
		void initOtherVegetation(int WORLD_SIZE, CraftWorld *world);

		void initLavaPools(int WORLD_SIZE, CraftWorld *world);
		void initBiome(int WORLD_SIZE, CraftWorld *world, PerlinModule* perlin, int seed);

		void initCanes(int WORLD_SIZE, CraftWorld *world, int waterLevel);
		void initBedrock(int WORLD_SIZE, CraftWorld *world);
        void initBeachSand(int WORLD_SIZE, CraftWorld *world);

        void initErosion(int WORLD_SIZE, CraftWorld *world);

        void initOre(int WORLD_SIZE, CraftWorld *world, int oreID, float oreConcentration, int oreLowestBoundary, int oreHighestBoundary, int variations);

		void initDungeons(int WORLD_SIZE, CraftWorld *world);

        void initClay(int WORLD_SIZE, CraftWorld *world);
		void initDirt(int WORLD_SIZE, CraftWorld *world);
		void initGravel(int WORLD_SIZE, CraftWorld *world);
		void initLavaLakes(int WORLD_SIZE, CraftWorld *world);
        void initWaterLakes(int WORLD_SIZE, CraftWorld *world);

        int GenerateOreVine(int WORLD_SIZE, CraftWorld *world, int x, int y, int z, int oreID, int type);

        void GeneratePileOfLeaves(CraftWorld *world, int x, int y, int z);
        void GenerateClassicTree(int WORLD_SIZE, CraftWorld *world, int x, int y, int z, int trunkBlock, int leavesBlock);
        void GenerateSpruceTree(int WORLD_SIZE, CraftWorld *world, int x, int y, int z);

        float InterpolateBiomeNoise(int wx, int wz);
        float InterpolateMoistureNoise(int wx, int wz);
        float InterpolateElevationNoise(int wx, int wz);
        float InterpolateRoughnessNoise(int wx, int wz);

        SimplexNoise func1,func2,func3;

        float GetValue3D(int wx, int wy, int wz, int octaves, float startFrequency, float startAmplitude);
        float GetValue2D(int wx, int wz, int octaves, float startFrequency, float startAmplitude);
        float GetValue2D2(int wx, int wz, int octaves, float startFrequency, float startAmplitude);
        float GetValue2D3(int wx, int wz, int octaves, float startFrequency, float startAmplitude);
        void SetSeed(int seed);
};

#endif
