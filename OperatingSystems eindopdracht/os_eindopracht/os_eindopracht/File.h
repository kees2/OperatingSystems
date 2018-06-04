#ifndef FILE_H_
#define FILE_H_
#include "Queue.h"
#include "Filter.h"

class File
{
private:
	const char * inputFile;
	const char * outputFile;
	int length;
	int count;
	signed short * outputBuf;
	Queue * bassQ;
	Queue * trebleQ;
	Queue * writeQ;
	Filter * filter;
	HANDLE bassCanGet, trebleCanGet, writeCanGet;
	std::mutex bassmtx, treblemtx, writemtx;
public:
	File(void);
	File(const char * new_input, const char * new_output, int bassIntensity, int trebleIntensity);
	~File(void);
	void read(void);
	void bassMod(void);
	void trebleMod(void);
	void writeBuf(void);
	void writeFile(void);
};
#endif
