#ifndef REINHARD_TONE_H
#define REINHARD_TONE_H

#include "tone.h"

#define GRAY	0.18

class ReinhardTone : public Tone
{
public:

	ReinhardTone() : Tone(LMAX, LDMAX), a(GRAY) {}
	ReinhardTone(double lM, double ldM) : Tone(lM, ldM), a(GRAY) {}
	~ReinhardTone() {}

	void reproduce(double avgLum, Colour& col)
	{
		//step 1
		col *= a / avgLum;

		//step 2
		col.r /= (1 + col.r);
		col.g /= (1 + col.g);
		col.b /= (1 + col.b);

		//step 3
		col *= ldMax;
	}

	/*BEGIN: Getters & Setters*/
	double getGrayVal()
	{
		return a;
	}
	void setGrayVal(double n)
	{
		a = n;
	}
	/*END: Getters & Setters*/

private:
	double a;	//percent for gray zone V
};

#endif