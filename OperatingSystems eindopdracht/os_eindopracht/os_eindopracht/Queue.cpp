#include "Queue.h"

Queue::Queue(void)
	: head(nullptr), tail(nullptr), count(0)
{
}

Queue::~Queue(void)
{
}

void Queue::insert(Block * new_block)
{
	mtx.lock();
	if (isEmpty())
		head = new_block;
	else
		tail->setNext(new_block);
	tail = new_block;
	count++;
	mtx.unlock();
}

Block* Queue::remove(void)
{
	mtx.lock();
	Block * block = nullptr;
	if (!isEmpty())
	{
		block = head;
		if (head->getNext() == nullptr)
			tail = nullptr;
		head = head->getNext();
		count--;
	}
	mtx.unlock();
	return block;
}

Block* Queue::peek(void)
{
	return head;
}

bool Queue::isEmpty(void)
{
	if (count == 0) {
		return 1;
	}
	else {
		return 0;
	}
	//return(head == nullptr);
}

int Queue::getCount(void)
{
	return count;
}
