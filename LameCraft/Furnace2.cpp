#include "Furnace2.h"


Furnace::Furnace(int x, int y, int z)
{
    for(int i = 0; i <= 2; i++)
    {
        furnaceSlotId[i] = -1;
        furnaceSlotAm[i] = -1;
        furnaceSlotSt[i] = 0;
    }

    furnaceX = x;
    furnaceZ = z;
    furnaceY = y;

    fuelTime = 0.0f;
    meltingTime = 0.0f;

    side = -1;
    smeltItemTemp = -1;
    working = false;
}

void Furnace::SetSide(char side2)
{
    side = side2;
}

char Furnace::GetSide()
{
    return side;
}

Furnace::~Furnace()
{

}

