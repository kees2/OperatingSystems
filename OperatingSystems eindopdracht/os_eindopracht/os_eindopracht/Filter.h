#ifndef FILTER_H_
#define FILTER_H_
#include "Block.h"

class Filter
{
private:
	double a1_bass, a2_bass, b0_bass, b1_bass, b2_bass;
	double a1_treble, a2_treble, b0_treble, b1_treble, b2_treble;
public:
	Filter(int bassIntensity, int trebleIntensity);
	virtual ~Filter(void);
	void bassCoefficients(int intensity);
	void trebleCoefficients(int intensity);
	void bassModulation(Block * block);
	void trebleModulation(Block * block);
};
#endif
