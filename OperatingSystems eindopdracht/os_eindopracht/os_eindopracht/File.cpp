#include <fstream>
#include "File.h"
#include <iostream>

int readingFinished = 0;
int bassFinished = 0;
int trebleFinished = 0;

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
	std::cout << "Reading has been finished " << std::endl;
	readingFinished = 1;
}

void File::bassMod(void)
{
	Block * block;
	int i = 0;
	bassmtx.lock();
	while(readingFinished != 1  || bassQ->isEmpty() == 0)
	{
		while (bassQ->isEmpty() && readingFinished != 1) {
			bassmtx.unlock();
			WaitForSingleObject(bassCanGet, 10);
			bassmtx.lock();
		}
		if (readingFinished != 1 && bassQ->isEmpty() == 0) {
			block = bassQ->remove();
			bassmtx.unlock();
			ResetEvent(bassCanGet);
			i++;
			filter->bassModulation(block);
			trebleQ->insert(block);
			SetEvent(trebleCanGet);
			bassmtx.lock();
		}
	}
	bassmtx.unlock();
	std::cout << "Bass thread is klaar" << std::endl;
	bassFinished = 1;
	

}

void File::trebleMod(void)
{
	Block * block;
	int i = 0;
	treblemtx.lock();
	while (bassFinished != 1 || trebleQ->isEmpty() == 0)
	{
		while (trebleQ->isEmpty() && bassFinished != 1) {
			treblemtx.unlock();
			WaitForSingleObject(trebleCanGet, 10);
			treblemtx.lock();
		}
		if (bassFinished != 1 && trebleQ->isEmpty() == 0) {
			block = trebleQ->remove();
			treblemtx.unlock();
			ResetEvent(trebleCanGet);
			i++;
			filter->trebleModulation(block);
			writeQ->insert(block);
			SetEvent(writeCanGet);
			treblemtx.lock();
		}
	}
	trebleFinished = 1;
	treblemtx.unlock();
	std::cout << "Treble thread is klaar" << std::endl;
}

void File::writeBuf(void)
{
	Block * block;
	int i = 0;
	writemtx.lock();
	while (trebleFinished != 1 || writeQ->isEmpty() == 0)
	{
		while (writeQ->isEmpty() && trebleFinished != 1) {
			writemtx.unlock();
			WaitForSingleObject(writeCanGet, 10);
			writemtx.lock();
		}
		if (trebleFinished != 1 && writeQ->isEmpty() == 0) {
			i++;
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
	std::cout << "Writing has been finished" << std::endl;
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
