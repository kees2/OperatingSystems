#ifndef BLOCK_H_
#define BLOCK_H_

class Block
{
private:
	int blockId;
	signed short data[1024];
	Block * next;
public:
	Block(void);
	Block(int id, signed short blockBytes[1024], Block * new_next);
	virtual ~Block(void);
	signed short * getData(void);
	int getId(void);
	void setNext(Block * new_next);
	Block * getNext(void);
};
#endif
