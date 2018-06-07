#include <fstream>
#include <iostream>
#include "File.h"

File::File(void)
{
}

File::File(const char * new_input, const char * new_output, int bassIntensity, int trebleIntensity)
{
	inputFile = new_input;
	outputFile = new_output;

	std::fstream fp;
	fp.open(inputFile, std::ios::in | std::ios::binary);
	fp.seekg(0, fp.end);
	length = fp.tellg();
	fp.seekg(0, fp.beg);
	length = length / 2;
	count = length / 1024;
	outputBuf = new signed short[length];

	bassQ = new Queue;
	trebleQ = new Queue;
	writeQ = new Queue;
	filter = new Filter(bassIntensity, trebleIntensity);

	bassCanGet = CreateEvent(0, 0, 0, 0);
	trebleCanGet = CreateEvent(0, 0, 0, 0);
	writeCanGet = CreateEvent(0, 0, 0, 0);

	readDone = bassDone = trebleDone = false;
}

File::~File(void)
{
}

void File::read(void)
{
	std::fstream fp;
	int dataCounter = 0;
	int blockId = 0;
	signed short dataBuffer;
	signed short data[1024];

	fp.open(inputFile, std::ios::in | std::ios::binary);
	while (fp.read((char *)&dataBuffer, sizeof(signed short))) {
		data[dataCounter] = dataBuffer;
		dataCounter++;
		if (dataCounter >= 1024) {
			Block * block = new Block(blockId, data, nullptr);
			bassmtx.lock();
			bassQ->insert(block);
			bassmtx.unlock();
			SetEvent(bassCanGet);
			blockId++;
			dataCounter = 0;
		}
	}
	readDone = true;
}

void File::bassMod(void)
{
	Block * block;
	bassmtx.lock();
	while(!readDone || !bassQ->isEmpty())
	{
		while (bassQ->isEmpty() && !readDone) {
			bassmtx.unlock();
			WaitForSingleObject(bassCanGet, 10);
			bassmtx.lock();
		}
		if (!readDone || !bassQ->isEmpty()) {
			block = bassQ->remove();
			bassmtx.unlock();
			ResetEvent(bassCanGet);
			filter->bassModulation(block);
			trebleQ->insert(block);
			SetEvent(trebleCanGet);
			bassmtx.lock();
		}
	}
	bassmtx.unlock();
	bassDone = true;
}

void File::trebleMod(void)
{
	Block * block;
	treblemtx.lock();
	while (!bassDone || !trebleQ->isEmpty())
	{
		while (trebleQ->isEmpty() && !bassDone) {
			treblemtx.unlock();
			WaitForSingleObject(trebleCanGet, 10);
			treblemtx.lock();
		}
		if (!bassDone || !trebleQ->isEmpty()) {
			block = trebleQ->remove();
			treblemtx.unlock();
			ResetEvent(trebleCanGet);
			filter->trebleModulation(block);
			writeQ->insert(block);
			SetEvent(writeCanGet);
			treblemtx.lock();
		}
	}
	trebleDone = true;
	treblemtx.unlock();
}

void File::writeBuf(void)
{
	Block * block;
	writemtx.lock();
	while (!trebleDone || !writeQ->isEmpty())
	{
		while (writeQ->isEmpty() && !trebleDone) {
			writemtx.unlock();
			WaitForSingleObject(writeCanGet, 10);
			writemtx.lock();
		}
		if (!trebleDone || !writeQ->isEmpty()) {
			block = writeQ->remove();
			writemtx.unlock();
			ResetEvent(writeCanGet);
			for (int j = 0; j < 1024; j++) {
				outputBuf[(block->getId() * 1024) + j] = block->getData()[j];
			}
			writemtx.lock();
		}
	}
	writemtx.unlock();
}

void File::writeFile(void)
{
	std::ofstream ofs;
	ofs.open(outputFile, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	std::ofstream stream(outputFile, std::ios::app | std::ios::binary | std::ios::out);
	for (int i = 0; i < length; i++)
		stream.write((char *)&(outputBuf[i]), sizeof(short));
}
