#ifndef DIRECTIONBLOCK_H
#define DIRECTIONBLOCK_H

class DirectionBlock
{
public:
	DirectionBlock(int x, int y, int z, char dir);
	virtual ~DirectionBlock();

	int X;
	int Y;
	int Z;

	char direction;
};

#endif
