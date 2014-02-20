#ifndef BLINN_PHONG_H
#define BLINN_PHONG_H

#include "illuminationmodel.h"

/**
 * BlinnPhong
 *
 * The Blinn-Phong illumination model; a variation of the Phong model, it uses
 * the vector half way between the eye vector and incoming light vector to
 * determine the specular component.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class BlinnPhong :
	public IlluminationModel
{
public:
	BlinnPhong() : IlluminationModel() {}
	BlinnPhong(Colour * amb) : IlluminationModel(amb) {}

	~BlinnPhong() {}

	Colour illuminate(vector<IntersectData> intersects) {
		Material& mat = intersects.front().material;

		Colour amb = (mat.getAmbient() * (*ambient)) * mat.getAmbientCoeff() * ka;
		Colour diffuse;
		Colour specular;

		vector<IntersectData>::iterator iter;
		for (iter = intersects.begin(); iter != intersects.end(); iter++) {
			if (iter->light != 0) {
				Vector3 halfway = iter->direction + iter->incoming;
				halfway.normalize();

				diffuse += iter->light->color * mat.getDiffuse() * (iter->incoming * iter->normal);
				float dot = iter->normal * halfway;
				if (dot >= 0) {
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