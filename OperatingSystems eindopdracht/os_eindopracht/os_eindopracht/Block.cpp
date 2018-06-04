#include "Block.h"

Block::Block(void)
	: blockId(0), next(nullptr)
{
}

Block::Block(int id, signed short blockBytes[1024], Block * new_next)
{
	blockId = id;
	for (int i = 0; i < 1024; i++) {
		data[i] = blockBytes[i];
	}
	next = new_next;
}

Block::~Block(void)
{
}

signed short * Block::getData(void)
{
	return data;
}

int Block::getId(void)
{
	return blockId;
}

void Block::setNext(Block * new_next)
{
	next = new_next;
}

Block * Block::getNext(void)
{
	return next;
}
