#ifndef FURNACE_H
#define FURNACE_H

class Furnace
{
public:
	Furnace(int x, int y, int z);

	void SetSide(char side2);
	char GetSide();

	virtual ~Furnace();

	int furnaceSlotId[3];
	int furnaceSlotAm[3];
	bool furnaceSlotSt[3];

	int smeltItemTemp;

	float fuelTime;
	float meltingTime;
	bool working;

	int furnaceX;
	int furnaceY;
	int furnaceZ;

	short id;
	char side;
};

#endif
