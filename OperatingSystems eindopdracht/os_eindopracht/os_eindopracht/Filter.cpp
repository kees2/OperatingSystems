#include <math.h>
#include "Filter.h"

Filter::Filter(int bassIntensity, int trebleIntensity)
{
	bassCoefficients(bassIntensity);
	trebleCoefficients(trebleIntensity);
}

Filter::~Filter()
{
}

void Filter::bassCoefficients(int intensity)
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

void Filter::trebleCoefficients(int intensity)
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

	a1_treble = (2.0*((a - 1) - (a + 1)*cos(w0))) / a0_treble;
	a2_treble = ((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0_treble;

	b0_treble = (a*((a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0_treble;
	b1_treble = (-2.0*a*((a - 1) + (a + 1)*cos(w0))) / a0_treble;
	b2_treble = (a*((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0_treble;
}

void Filter::bassModulation(Block * block)
{
	for (int i = 0; i < 1024; i++) {
		if (i > 2)
			block->getData()[i] = (signed short)(b0_bass * block->getData()[i]) + (b1_bass * block->getData()[i - 1]) + (b2_bass * block->getData()[i - 2]) - (a1_bass*block->getData()[i - 1]) - (a2_bass * block->getData()[i - 2]);
		else
			block->getData()[i] = 0;
	}
}

void Filter::trebleModulation(Block * block)
{
	for (int i = 0; i < 1024; i++) {
		if (i > 2)
			block->getData()[i] = (signed short)(b0_treble * block->getData()[i]) + (b1_treble * block->getData()[i - 1]) + (b2_treble * block->getData()[i - 2]) - (a1_treble*block->getData()[i - 1]) - (a2_treble * block->getData()[i - 2]);
		else
			block->getData()[i] = 0;
	}
}
