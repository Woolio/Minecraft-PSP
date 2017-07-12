#ifndef JUKEBOXENTITY_H
#define JUKEBOXENTITY_H

#include "BlockEntity.h"

class JukeboxEntity : public BlockEntity
{
public:
	JukeboxEntity(int create_x, int create_y, int create_z, int create_record);
	virtual ~JukeboxEntity();

	int GetRecord();
	void SetRecord(int new_record);

private:
    int record;
};

#endif
