#include "NoteBlockEntity.h"

NoteBlockEntity::NoteBlockEntity(int create_x, int create_y, int create_z, int create_note) : BlockEntity(create_x, create_y, create_z)
{
    note = create_note;
}

int NoteBlockEntity::GetNote()
{
    return note;
}

float NoteBlockEntity::GetPitch()
{
    if(note <= 12)
    {
        return (note/24.0f) + 0.5f;
    }
    return ((note-12)/12.0f) + 1.0f;
}

void NoteBlockEntity::IncreaseNote()
{
    note ++;
    if(note == 25)
    {
        note = 0;
    }
}

NoteBlockEntity::~NoteBlockEntity()
{

}

