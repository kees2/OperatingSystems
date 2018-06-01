#include "stdafx.h"
#include "fileHandler.h"
#include <string>
#include <iostream>
#include "Block.h"
#include <fstream>
#include "Queue.h"
#include "filters.h"
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
	
	Filters filter(6, 6);

	fp.open(fileName, ios::in | ios::binary);
	Queue blockqueue;
	
	int dataCounter = 0;
	int blockId = 0;
	int16_t dataBuffer;
	int16_t data[1024];
	
	fp.seekg(0, fp.end);
	int length = fp.tellg();
	fp.seekg(0, fp.beg);
	length = length / 2;
	
	while (fp.read((char *)&dataBuffer, sizeof(int16_t))) {	//(char *)
		
		
		data[dataCounter] = dataBuffer;
		
		dataCounter++;

		if (dataCounter >= 1024) {
			
			Block block(blockId, data);
			
			filter.bassModulation(&block);
			filter.trebleModulation(&block);
			writeBlockToBuffer(&block);
			

			blockId++;
			dataCounter = 0;
		}
		
	}

}

void fileHandler::writeBlockToBuffer(Block *block) {
	for (int i = 0; i < 1024; i++) {
		outputBuf[(block->getId() * 1024) + i] = block->getData()[i];
	}
}

void fileHandler::writeFile(char* fileName) {

	ofstream ofs;
	ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
	ofs.close();

	

	ofstream stream(fileName, std::ios::app | std::ios::binary | std::ios::out);
	int i = 0;
	while (i < 2742142) {
		stream.write((char *)&(outputBuf[i]), sizeof(short));
		i++;
	}

	printf("Writing to file has been completed\n");
	
}
