#pragma once
#include "inttypes.h"

class fileHandler
{
public:
	fileHandler();
	~fileHandler();

	void readFile(char* fileName);
	void writeFile(char* fileName);
private:
	int16_t * outputBuf;
	int16_t * inputBuf;
	int fileSize;
};

