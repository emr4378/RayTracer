#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "vecmath.h"
#include "colour.h"

/**
 * LightSource
 *
 * A basic representation of a point light, storing only the essentials.
 *
 * @author Eduardo Rodrigues
 */
class LightSource
{
public:
	LightSource() : position(), color() {}
	LightSource(Point3 p, Colour c) : position(p), color(c) {}
	~LightSource() {}

	Point3 position;
	Colour color;
};

#endif