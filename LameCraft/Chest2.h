#ifndef CHEST_H
#define CHEST_H

class Chest
{
public:
	Chest(int x, int y, int z);
	virtual ~Chest();

	int chestSlotId[27];
	int chestSlotAm[27];
	bool chestSlotSt[27];

	int chestAddress;

	short id;
};

#endif
