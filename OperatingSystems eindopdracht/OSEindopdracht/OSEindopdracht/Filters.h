#pragma once
class Filters
{
public:
	Filters();
	~Filters();
	void bassCoefficients(int intensity, double *b0, double *b1, double*b2, double *a1, double *a2);
	void trebleCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2);
};

