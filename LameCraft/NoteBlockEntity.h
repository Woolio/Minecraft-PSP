#ifndef NOTEBLOCKENTITY_H
#define NOTEBLOCKENTITY_H

#include "BlockEntity.h"

class NoteBlockEntity : public BlockEntity
{
public:
	NoteBlockEntity(int create_x, int create_y, int create_z, int create_note);
	virtual ~NoteBlockEntity();

	int GetNote();
	float GetPitch();
	void IncreaseNote();

private:
    int note;
};

#endif
