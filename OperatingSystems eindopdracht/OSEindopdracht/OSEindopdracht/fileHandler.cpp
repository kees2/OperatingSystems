#include "stdafx.h"
#include "fileHandler.h"
#include <string>
#include <iostream>
#include "Block.h"
#include <fstream>
using namespace std;

#pragma warning(disable:4996)


fileHandler::fileHandler()
{
	
}


fileHandler::~fileHandler()
{
}

void fileHandler::readFile(char* fileName) {
	fstream fp;

	//FILE *fp;
	//fp = fopen(fileName, "r");

	//fseek(fp, 0, SEEK_END);
	//fileSize = ftell(fp);
	//fseek(fp, 0, SEEK_SET);

	fp.open(fileName, ::ios::in | ios::binary);



	inputBuf = new int16_t[fileSize];
	outputBuf = new int16_t[fileSize];
	
	int dataCounter = 0;
	int blockId = 0;
	int16_t dataBuffer;
	int16_t data[1024];

	while (fp.read((char *)&dataBuffer, sizeof(int16_t))) {	//(char *)
		outputBuf[dataCounter] = dataBuffer;
		if (dataCounter < fileSize) {
			dataCounter++;
		}
		else {
			printf("warning");
		}
		//dataCounter++;

		//data[dataCounter] = dataBuffer;
		if (dataCounter == 1024) {
			Block block(blockId, data);
			blockId++;
			
		}
	}
}

void fileHandler::writeFile(char* fileName) {
	FILE *fp;
	fp = fopen(fileName, "w");
	fwrite(outputBuf, sizeof(int16_t), fileSize, fp);
}
