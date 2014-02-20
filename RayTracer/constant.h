#ifndef CONSTANT_H
#define CONSTANT_H

#include "illuminationmodel.h"

/**
 * Constant
 *
 * The Constant, or Diffuse, illumination model. Illuminations objects only
 * using the diffuse components of Materials and LightSources.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Constant :
	public IlluminationModel
{
public:
	Constant() : IlluminationModel() {}
	~Constant() {}

	Colour illuminate(vector<IntersectData> intersects)
	{
		Material& mat = intersects.front().material;

		Colour diffuse;

		vector<IntersectData>::iterator iter;
		for (iter = intersects.begin(); iter != intersects.end(); iter++)
		{
			if (iter->light != 0)
			{
				diffuse += iter->light->color * mat.getDiffuse() * (iter->incoming * iter->normal) * kd;
			}
		}

		return diffuse;
	}
};
#endif