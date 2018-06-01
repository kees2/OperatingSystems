#pragma once
#include "inttypes.h"
class Block
{
public:
	Block(int id, int16_t blockBytes[1024]);
	~Block();
	Block();
	int16_t * getData();
	int getId();
	void printBlock();
private:
	int blockId = 0;
	int16_t data[1024];
};

