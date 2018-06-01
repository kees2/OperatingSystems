#pragma once
using namespace std;
#include <string>
#include "Block.h"

class Queue
{
    public:
        Queue();
        virtual ~Queue();
        void insert(Block j);
		Block remove();
        int isEmpty();
        int isFull();
        int size();

    protected:

    private:
        int maxSize = 50;
		Block queArray[50];
        int front;
        int rear;
        int numberItems;

};


