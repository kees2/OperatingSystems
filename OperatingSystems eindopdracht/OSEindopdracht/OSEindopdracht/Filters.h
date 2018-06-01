#pragma once
#include "inttypes.h"
#include "Block.h"

class Filters
{
private:

public:
	Filters(int bassIntensity, int trebleIntensity);
	~Filters();
	void bassCoefficients(int intensity);
	void trebleCoefficients(int intensity);
	void bassModulation(Block * block);
	void trebleModulation(Block * block);

	double a1_bass, a2_bass, b0_bass, b1_bass, b2_bass;
	double a1_treble, a2_treble, b0_treble, b1_treble, b2_treble;
};