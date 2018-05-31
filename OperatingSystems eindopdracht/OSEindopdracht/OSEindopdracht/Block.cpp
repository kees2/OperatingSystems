#include "stdafx.h"
#include "Block.h"

Block::Block(int id, int16_t blockBytes[1024])
{
	blockId = id;
	data = new int[1024];
	for (int i = 0; i < 1024; i++) {
		data[i] = blockBytes[i];
	}
}

Block::~Block()
{
	delete[] data;
}

int16_t * Block::getData() {
	return this->data;
}
