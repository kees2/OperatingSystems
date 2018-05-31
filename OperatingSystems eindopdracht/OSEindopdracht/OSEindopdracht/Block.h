#pragma once
#include "inttypes.h"
class Block
{
public:
	Block(int id, int16_t blockBytes[1024]);
	virtual ~Block(void);
	int16_t * getData();
private:
	int blockId;
	int16_t * data;
};
