#ifndef WARD_TONE_H
#define WARD_TONE_H

#include "tone.h"

#define	LMAX	1000
#define	LDMAX	100

class WardTone : public Tone
{
public:

	WardTone() : Tone(LMAX, LDMAX) {}
	WardTone(float lM, float ldM) : Tone(lM, ldM) {}
	~WardTone() {}

	void reproduce(float avgLum, Colour& col) {
		float scale;

		scale =	 ( 1.219 + pow(ldMax/2.0f, 0.4f) );
		scale /= ( 1.219 + pow(avgLum, 0.4f) );
		scale = pow(scale, 2.5f);

		col *= scale;
	}
};

#endif