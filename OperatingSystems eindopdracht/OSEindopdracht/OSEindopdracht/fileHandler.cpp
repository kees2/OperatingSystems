#include "stdafx.h"
#include "fileHandler.h"
#include <string>
#include <iostream>
#include "Block.h"
#include <fstream>
#include "Queue.h"
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

	fp.open(fileName, ios::in | ios::binary);
	Queue blockqueue;


	inputBuf = new int16_t[5484284/2];
	outputBuf = new int16_t[5484284/2];
	
	int dataCounter = 0;
	int blockId = 0;
	int16_t dataBuffer;
	int16_t data[1024];

	fp.seekg(0, fp.end);
	int length = fp.tellg();
	fp.seekg(0, fp.beg);
	length = length / 2;

	while (fp.read((char *)&dataBuffer, sizeof(int16_t))) {	//(char *)
		
		
		outputBuf[dataCounter] = dataBuffer;

		if (dataCounter == 1024) {
			Block block(blockId, data);
			blockId++;
			blockqueue.insert(block);
		}

		dataCounter++;

		/*if (dataCounter < length) {
			dataCounter++;
		}
		else {
			printf(".");
		}*/
		//dataCounter++;

		//data[dataCounter] = dataBuffer;
		/*if (dataCounter == 1024) {
			Block block(blockId, data);
			blockId++;
			
		}*/
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
