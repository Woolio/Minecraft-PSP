#include "MonsterSpawnerEntity.h"

MonsterSpawnerEntity::MonsterSpawnerEntity(int create_x, int create_y, int create_z, std::string create_mobToSpawn) : BlockEntity(create_x, create_y, create_z)
{
    mobToSpawn = "";
    mobToSpawn = create_mobToSpawn;
    forceSpawn = false;

    particleDelay1 = (rand() % 500)/1000.0f;
    particleDelay2 = (rand() % 500)/1000.0f;

    minSpawnDelay = 20.0f;
    maxSpawnDelay = 40.0f;

    minSpawnCount = 1;
    maxSpawnCount = 2;

    delay = 5.0f;

    actArea = BoundingBox(Vector3(GetX()-10,GetY()-6,GetZ()-10),Vector3(GetX()+10,GetY()+6,GetZ()+10));
}

void MonsterSpawnerEntity::SetStandardProperties()
{
    minSpawnDelay = 20.0f;
    maxSpawnDelay = 40.0f;

    minSpawnCount = 1;
    maxSpawnCount = 2;

    delay = 5.0f;

    actArea = BoundingBox(Vector3(GetX()-10,GetY()-6,GetZ()-10),Vector3(GetX()+10,GetY()+6,GetZ()+10));
}

bool MonsterSpawnerEntity::ItIsTimeToSpawn()
{
    return forceSpawn;
}

std::string MonsterSpawnerEntity::MobToSpawn()
{
    return mobToSpawn;
}

int MonsterSpawnerEntity::MobCountToSpawn()
{
    return minSpawnCount+rand()%(maxSpawnCount-minSpawnCount+1);
}

void MonsterSpawnerEntity::Update(float dt)
{
    delay -= dt;
    particleDelay1 -= dt;
    particleDelay2 -= dt;

    if(delay <= 0.0f)
    {
        delay = minSpawnDelay + rand() % ((int)(maxSpawnDelay-minSpawnDelay+1));

        forceSpawn = true;
    }

}

MonsterSpawnerEntity::~MonsterSpawnerEntity()
{

}

