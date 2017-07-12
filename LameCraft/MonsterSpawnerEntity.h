#ifndef MONSTERSPAWNERENTITY_H
#define MONSTERSPAWNERENTITY_H

#include "BlockEntity.h"

#include <string>
#include <pspmath.h>
#include <psptypes.h>
#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Math/BoundingBox.h>

class MonsterSpawnerEntity : public BlockEntity
{
public:
	MonsterSpawnerEntity(int create_x, int create_y, int create_z, std::string create_mobToSpawn);
	virtual ~MonsterSpawnerEntity();

	void Update(float dt);
	void SetStandardProperties();

	bool ItIsTimeToSpawn();
	std::string MobToSpawn();
	int MobCountToSpawn();

    BoundingBox actArea;
    bool forceSpawn;

    float particleDelay1;
    float particleDelay2;

private:
    std::string mobToSpawn;

    float delay;
    float minSpawnDelay;
    float maxSpawnDelay;

    int minSpawnCount;
    int maxSpawnCount;
};

#endif
