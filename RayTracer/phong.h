#ifndef PHONG_H
#define PHONG_H

#include "illuminationmodel.h"

/**
 * Phong
 *
 * A classic illumination model, Phong illumination introduces a specular
 * component. This component causes a specular highlight (default white)
 * to be visible at certain viewing angles, giving the appearance of
 * high reflactance or shininess.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Phong : public IlluminationModel {
public:
	Phong() : IlluminationModel() {}

	Phong(Colour * amb) : IlluminationModel(amb) {}

	~Phong() {}

	Colour illuminate(vector<IntersectData> intersects) {
		Material& mat = intersects.front().material;

		Colour amb = (mat.getAmbient() * (*ambient)) * mat.getAmbientCoeff() * ka;
		Colour diffuse;
		Colour specular;

		vector<IntersectData>::iterator iter;
		for (iter = intersects.begin(); iter != intersects.end(); iter++) {
			if (iter->light != 0) {
				diffuse += iter->light->color * mat.getDiffuse() * (iter->incoming * iter->normal);
				float dot = iter->reflection * iter->direction;
				if (dot <= 0) {
					specular += iter->light->color * mat.getSpecular() * powf(dot, mat.getSpecularSizeCoeff()) * powf(dot, ke);
				}
			}
		}

		diffuse *= mat.getDiffuseCoeff() * kd;
		specular *= mat.getSpecularCoeff() * ks;

		return (amb + diffuse + specular);
	}
};

#endif