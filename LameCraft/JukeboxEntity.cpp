#include "JukeboxEntity.h"

JukeboxEntity::JukeboxEntity(int create_x, int create_y, int create_z, int create_record) : BlockEntity(create_x, create_y, create_z)
{
    record = create_record;
}

int JukeboxEntity::GetRecord()
{
    return record;
}

void JukeboxEntity::SetRecord(int new_record)
{
    record = new_record;
}

JukeboxEntity::~JukeboxEntity()
{

}

