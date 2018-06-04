#include <fstream>
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
			bassQ->insert(block);
			SetEvent(bassCanGet);
			blockId++;
			dataCounter = 0;
		}
	}
}

void File::bassMod(void)
{
	Block * block;
	int i = 0;
	while (1)
	{
		bassmtx.lock();
		while (bassQ->isEmpty()) {
			bassmtx.unlock();
			WaitForSingleObject(bassCanGet, INFINITE);
			bassmtx.lock();
		}
		block = bassQ->remove();
		bassmtx.unlock();
		ResetEvent(bassCanGet);
		i++;
		filter->bassModulation(block);
		trebleQ->insert(block);
		SetEvent(trebleCanGet);
	}
}

void File::trebleMod(void)
{
	Block * block;
	int i = 0;
	while (1)
	{
		treblemtx.lock();
		while (trebleQ->isEmpty()) {
			treblemtx.unlock();
			WaitForSingleObject(trebleCanGet, INFINITE);
			treblemtx.lock();
		}
		block = trebleQ->remove();
		treblemtx.unlock();
		ResetEvent(trebleCanGet);
		i++;
		filter->trebleModulation(block);
		writeQ->insert(block);
		SetEvent(writeCanGet);
	}
}

void File::writeBuf(void)
{
	Block * block;
	int i = 0;
	while (1)
	{
		writemtx.lock();
		while (writeQ->isEmpty()) {
			writemtx.unlock();
			WaitForSingleObject(writeCanGet, INFINITE);
			writemtx.lock();
		}
		i++;
		block = writeQ->remove();
		writemtx.unlock();
		ResetEvent(writeCanGet);
		for (int j = 0; j < 1024; j++)
			outputBuf[(block->getId() * 1024) + j] = block->getData()[j];
	}
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
