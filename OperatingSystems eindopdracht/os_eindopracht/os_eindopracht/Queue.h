#ifndef QUEUE_H_
#define QUEUE_H_
#include <mutex>
#include <Windows.h>
#include "Block.h"

class Queue
{
private:
	Block * head;
	Block * tail;
	int count;
	std::mutex mtx;
public:
	Queue(void);
	virtual ~Queue(void);
	void insert(Block * new_block);
	Block * remove(void);
	Block * peek(void);
	bool isEmpty(void);
	int getCount(void);
};

#endif
