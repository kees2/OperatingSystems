#include "stdafx.h"
#include "Filters.h"
#include <iostream>
#include <fstream>
#include "math.h"

#pragma warning(disable:4996)

Filters::Filters()
{
}


Filters::~Filters()
{
}

void Filters::bassCoefficients(int intensity, double *b0, double *b1, double*b2, double *a1, double *a2)
{
	double frequency = 330;
	double qFactor = 0.5;
	double gain = intensity;
	double sampleRate = 44100;

	double pi = 4.0*atan(1);
	double a = pow(10.0, gain / 40);
	double w0 = 2 * pi*frequency / sampleRate;
	double alpha = sin(w0) / (2.0*qFactor);
	double a0 = (a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha;

	*a1 = -(-2.0*((a - 1) + (a + 1)*cos(w0))) / a0;
	*a2 = -((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0;

	*b0 = (a*((a + 1) - (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0;
	*b1 = (2 * a*((a - 1) - (a + 1)*cos(w0))) / a0;
	*b2 = (a*((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0;
}
void Filters::trebleCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2)
{
	double frequency = 3300;
	double qFactor = 0.5;
	double gain = intensity;
	double sampleRate = 44100;

	double pi = 4.0*atan(1);
	double a = pow(10.0, gain / 40);
	double w0 = 2 * pi*frequency / sampleRate;
	double alpha = sin(w0) / (2.0*qFactor);
	double a0 = (a + 1) - (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha;

	*a1 = -(2.0*((a - 1) - (a + 1)*cos(w0))) / a0;
	*a2 = -((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0;

	*b0 = (a*((a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0;
	*b1 = (-2.0*a*((a - 1) + (a + 1)*cos(w0))) / a0;
	*b2 = (a*((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0;
}

int16_t * Filters::bassBlockModulation(Block block) {
	printf("In bass modulation");
	int16_t * dataInBuffer = new int16_t[1024];
	int16_t * dataOutBuffer = new int16_t[1024];
	dataInBuffer = block.getData();
	for (int i = 0; i < 1024; i++) {
		printf("%d", dataInBuffer[i]);
	}
	
	return dataOutBuffer;
}