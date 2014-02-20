#ifndef FLAT_H
#define FLAT_H

#include "illuminationmodel.h"

/**
 * Flat
 *
 * The easiest illumination model, Flat illumination uses only the ambient
 * component of materials and lights illuminate the scene.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Flat :
	public IlluminationModel
{
public:
	Flat() : IlluminationModel() {}

	~Flat() {}

	Colour illuminate(vector<IntersectData> intersects)
	{
		Material& mat = intersects.front().material;
		return (mat.getAmbient() * (*ambient));
	}
};

#endif