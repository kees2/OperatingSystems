// OSEindopdracht.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "filters.h"

#define SAMPLE_RATE 44100 // Hz
#define BITS_PER_SAMPLE 16 // bits
#define N_SAMPLES 16000 // n

int readPCM(int *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp);
int convertBitSize(unsigned int in, int bps);

using namespace std;
#include <io.h>
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	cout << "Das program hat angefangen" << endl;
	double b0 = 0, b1 = 0, b2 = 0, a1, a2;
	Filters filter;

	printf("SAMPLE RATE = [%d] BITS PER SAMPLE = [%d] N SAMPLES = [%d]\n", SAMPLE_RATE, BITS_PER_SAMPLE, N_SAMPLES);

	int *data = (int *)malloc(N_SAMPLES * sizeof(int));
	int *outData = (int *)malloc(N_SAMPLES * sizeof(int));

	readPCM(data, SAMPLE_RATE, BITS_PER_SAMPLE, N_SAMPLES);


	unsigned int i;
	for (i = 0; i < N_SAMPLES; ++i) {
		printf("%d\n", data[i]);
	}

	filter.bassCoefficients(100, &b0, &b1, &b2, &a1, &a2);

	for (int i = 3; i < N_SAMPLES; ++i) {
		outData[i] = (b0 * data[i]) + (b1 * data[i - 1]) + (b2 * data[i - 2]) - (a1*outData[i - 1]) - (a2*outData[i - 2]);
	}



	cin.get();
	return 0;
}

int convertBitSize(unsigned int in, int bps)
{
	const unsigned int max = (1 << (bps - 1)) - 1;
	return in > max ? in - (max << 1) : in;
}

int readPCM(int *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp)

{
	FILE *fp;
	unsigned char buf;
	unsigned int i, j;

	fp = fopen("you_and_i.pcm", "r");

	for (i = 0; i < num_samp; ++i) {

		unsigned int tmp = 0;

		for (j = 0; j != BITS_PER_SAMPLE; j += 8) {
			fread(&buf, 1, 1, fp);
			tmp += buf << j;
		}

		data[i] = convertBitSize(tmp, BITS_PER_SAMPLE);

	}
	return 1;
}

//deze verder af fixen
void writePCM(int *data, unsigned int samp_rate, unsigned int bits_per_samp, unsigned int num_samp){

	FILE *fp;
	unsigned int i, j;

	fp = fopen("mixedFile.pcm", "r");

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