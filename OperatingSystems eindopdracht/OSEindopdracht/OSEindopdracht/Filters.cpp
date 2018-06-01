#include "stdafx.h"
#include "Filters.h"
#include <iostream>
#include <fstream>
#include "math.h"

#pragma warning(disable:4996)

Filters::Filters(int bassIntensity, int trebleIntensity)
{
	bassCoefficients(bassIntensity);
	trebleCoefficients(trebleIntensity);
}

Filters::~Filters()
{
}

void Filters::bassCoefficients(int intensity)
{
	double frequency = 330;
	double qFactor = 0.5;
	double gain = intensity;
	double sampleRate = 44100;

	double pi = 4.0*atan(1);
	double a = pow(10.0, gain / 40);
	double w0 = 2 * pi*frequency / sampleRate;
	double alpha = sin(w0) / (2.0*qFactor);
	double a0_bass = (a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha;

	a1_bass = (-2.0*((a - 1) + (a + 1)*cos(w0))) / a0_bass;
	a2_bass = ((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0_bass;

	b0_bass = (a*((a + 1) - (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0_bass;
	b1_bass = (2 * a*((a - 1) - (a + 1)*cos(w0))) / a0_bass;
	b2_bass = (a*((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0_bass;
}

void Filters::trebleCoefficients(int intensity)
{
	double frequency = 3300;
	double qFactor = 0.5;
	double gain = intensity;
	double sampleRate = 44100;

	double pi = 4.0*atan(1);
	double a = pow(10.0, gain / 40);
	double w0 = 2 * pi*frequency / sampleRate;
	double alpha = sin(w0) / (2.0*qFactor);
	double a0_treble = (a + 1) - (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha;

	a1_treble = -(2.0*((a - 1) - (a + 1)*cos(w0))) / a0_treble;
	a2_treble = -((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0_treble;

	b0_treble = (a*((a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0_treble;
	b1_treble = (-2.0*a*((a - 1) + (a + 1)*cos(w0))) / a0_treble;
	b2_treble = (a*((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0_treble;
}

/*
void Filters::bassModulation(Block * block) {
	int16_t oldData[1024];
	int16_t newData[1024];
	for (int i = 0; i < 1024; i++) {
		oldData[i] = block->getData()[i];
		if (i > 2) {
			newData[i] = ((oldData[i] * b0_bass) + (oldData[i - 1] * b1_bass) + (oldData[i - 2] * b2_bass) - (newData[i - 1] * a1_bass) - (newData[i - 2] * a2_bass));
		}
		else {
			newData[i] = 0;
		}
		printf("%d == %d\n", oldData[i], newData[i]);
		block->getData()[i] = newData[i];
	}
}*/

void Filters::bassModulation(Block *block) {
	signed short dataIn[1024];
	signed short dataOut[1024];

	for (int i = 0; i < 1024; i++) {
		dataIn[i] = block->getData()[i];
		dataOut[i] = 0;
		if (i > 2) {
			dataOut[i] = (b0_bass * dataIn[i]) + (b1_bass * dataIn[i - 1]) + (b2_bass * dataIn[i - 2]) - (a1_bass*dataOut[i - 1]) - (a2_bass * dataOut[i - 2]);
		}
		block->getData()[i] = dataOut[i];
	}
	if (block->getId() == 1) {
		block->printBlock();
	}
}



void Filters::trebleModulation(Block * block) {
	for (int i = 0; i < 1024; i++) {
		if (i > 2)
			block->getData()[i] = ((block->getData()[i] * b0_treble) + (block->getData()[i - 1] * b1_treble) + (block->getData()[i - 1] * b2_treble) + (block->getData()[i - 1] * a1_treble) + (block->getData()[i - 1] * a2_treble));
		else
			block->getData()[i] = 0;
	}
}