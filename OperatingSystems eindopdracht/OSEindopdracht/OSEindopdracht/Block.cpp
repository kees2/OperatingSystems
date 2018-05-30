#include "stdafx.h"
#include "Block.h"



Block::Block(int id, int16_t blockBytes[1024])
{
	blockId = id;
	for (int i = 0; i < 1024; i++) {
		data[i] = blockBytes[i];
	}
}


Block::~Block()
{
}

Block::Block()
{
}

int16_t * Block::getData() {
	return this->data;
}