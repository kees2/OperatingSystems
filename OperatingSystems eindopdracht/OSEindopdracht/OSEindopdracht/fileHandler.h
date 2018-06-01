#pragma once
#include "inttypes.h"
#include "Block.h"

class fileHandler
{
public:
	fileHandler();
	~fileHandler();

	void readFile(char* fileName);
	void writeFile(char* fileName);
	void writeBlockToBuffer(Block *block);
private:
	int16_t * inputBuf = new int16_t[5484284 / 2];
	int16_t * outputBuf = new int16_t[5484284 / 2];;
	int fileSize;
};

