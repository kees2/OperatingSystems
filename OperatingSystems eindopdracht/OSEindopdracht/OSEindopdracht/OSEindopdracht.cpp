// OSEindopdracht.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "filters.h"
#include "Inttypes.h"
#include "fileHandler.h"
#include <string>
#include <ctype.h>

#define SAMPLE_RATE 44100 // Hz
#define BITS_PER_SAMPLE 16 // bits
#define N_SAMPLES 1028 // n

int readPCM(int16_t *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp);
void writePCM(int16_t *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp);
int convertBitSize(unsigned int in, int bps);

using namespace std;
#include <io.h>
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	fileHandler file1;
	int threads;
	int bassIntensity;
	int trebleIntensity;
	char * inputFile;
	char * outputFile;

	for (int i = 0; i < argc; i += 2) {
		std::cout << argv[i] << std::endl;

		string temp = argv[i];


		if (temp == "p") {
			if (((int)argv[i + 1] >= 1) && ((int)argv[i + 1] <= 8)) {
				threads = argv[i + 1]
			}
			else {
				std::cout << "aantal threads moet 1 t/m 8 zijn" << std::endl;
			}
		}
		else if (temp == "b") {
			if (((int)argv[i + 1] >= -6) && ((int)argv[i + 1] <= 6)) {
				bassIntensity = argv[i + 1];
			}
			else {
				std::cout << "bass intensity moet -6 t/m 6 zijn" << std::endl;
			}
		}
		else if (temp == "t") {
			if (((int)argv[i + 1] >= -6) && ((int)argv[i + 1] <= 6)) {
				trebleIntensity = argv[i + 1];
			}
			else {
				std::cout << "treble intensity moet -6 t/m 6 zijn" << std::endl;
			}
		}
		else if (temp == "inputFile") {
				inputFile = argv[i + 1];
		}
		else if (temp == "outputFile") {
				outputFile = argv[i + 1];
		}

	}
	
	

	/*
	file1.readFile("you_and_i.pcm");
	file1.writeFile("OutFile.pcm");

	*/
	cin.get();
	return 0;
}

int convertBitSize(unsigned int in, int bps)
{
	const unsigned int max = (1 << (bps - 1)) - 1;
	return in > max ? in - (max << 1) : in;
}

int readPCM(int16_t *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp)

{
	FILE *fp, *fp2;
	int16_t buf;
	unsigned int i, j;

	fp = fopen("you_and_i.pcm", "r");
	fp2 = fp = fopen("mixedFile.pcm", "w");


	for (i = 0; i < num_samp; ++i) {

		unsigned int tmp = 0;

		for (j = 0; j != BITS_PER_SAMPLE; j += 8) {
			fread(&buf, 1, 1, fp);
			tmp += buf << j;

			fwrite(&buf, 1, 1, fp2);
		}
		printf("%d \n", tmp);
		//printf("%d\n", tmp);
		//data[i] = convertBitSize(tmp, BITS_PER_SAMPLE);
		data[i] = tmp;

	}
	return 1;
}

//deze verder af fixen
void writePCM(int16_t *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp) {

	FILE *fp;
	unsigned int i, j;

	fp = fopen("mixedFile.pcm", "w");

	for (i = 0; i < num_samp; ++i) {

		unsigned int tmp = 0;

		for (j = 0; j != BITS_PER_SAMPLE; j += 8) {
			fwrite(data, 2, 1, fp);
		}


	}



}










/*

for (int i = 0; i < argc; i++) {
cout << argv[i] << endl;
if (argv[i][0] == 'k') {
cout << "k is ingegeven jij engert" << endl;
cout << "Tweede argument is " << argv[i][1] << endl;
}
}
cin.get();
FILE * infile = fopen("you_and_i.pcm", "rb");     // Open wave file in read
fseek(infile, 0, SEEK_END);
int fileSize = ftell(infile);
fseek(infile, 0, SEEK_SET);
signed short * buf = (signed short *)malloc(fileSize);
signed short * f = (signed short *)malloc(fileSize * sizeof(signed short));
//read buffer
fread(buf, sizeof(signed short), fileSize, infile);
*/