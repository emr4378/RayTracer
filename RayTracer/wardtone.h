#ifndef WARD_TONE_H
#define WARD_TONE_H

#include "tone.h"

class WardTone : public Tone
{
public:

	WardTone() : Tone(LMAX, LDMAX) {}
	WardTone(double lM, double ldM) : Tone(lM, ldM) {}
	~WardTone() {}

	void reproduce(double avgLum, Colour& col)
	{
		double scale;

		scale = (1.219 + pow(ldMax / 2.0, 0.4));
		scale /= (1.219 + pow(avgLum, 0.4));
		scale = pow(scale, 2.5);

		col *= scale;
	}
};

#endif