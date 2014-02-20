#ifndef COOK_TORRANCE_H
#define COOK_TORRANCE_H

#include "illuminationmodel.h"

/**
 * CookTorrance
 *
 * The Cook-Torrance illumination model; a fairly complex illumination model,
 * it uses a distribution function to spread out the specular highlight and
 * make the illumination look more realistic.
 *
 * Unfortunately, this makes it a bit more computationally expensive than other
 * illumination models.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class CookTorrance :
	public IlluminationModel
{
public:
	enum RoughMode { ROUGHMODE_BECKMANN, ROUGHMODE_GAUSSIAN };

	CookTorrance() : IlluminationModel(), roughValue(0.5), roughMode(ROUGHMODE_BECKMANN) {}

	CookTorrance(double rVal, RoughMode rMode) : IlluminationModel(), roughValue(rVal), roughMode(rMode) {}

	CookTorrance(Colour* amb) : IlluminationModel(amb) {}

	~CookTorrance() {}

	Colour illuminate(vector<IntersectData> intersects)
	{
		Material& mat = intersects.front().material;

		Colour amb = (mat.getAmbient() * (*ambient)) * mat.getAmbientCoeff() * ka;
		Colour diffuse;
		Colour specular;

		double fresnelTerm = intersects.front().direction * intersects.front().normal;

		vector<IntersectData>::iterator iter;
		for (iter = intersects.begin(); iter != intersects.end(); iter++)
		{
			if (iter->light != 0)
			{
				diffuse += iter->light->color * mat.getDiffuse() * (iter->incoming * iter->normal);


				Vector3 halfway = iter->direction + iter->incoming;
				halfway.normalize();

				double NdotL	= iter->normal * iter->incoming;
				double NdotH = iter->normal * halfway;
				double NdotV = iter->normal * iter->direction;
				double VdotH = iter->direction * halfway;
				double rSq = roughValue * roughValue;


				double temp1 = (2 * NdotH * NdotV) / VdotH;
				double temp2 = (2 * NdotH * NdotL) / VdotH;
				double geo = min(1.0, min(temp1, temp2));

				double roughness;

				if (roughMode == ROUGHMODE_BECKMANN)
				{
					double rA = 1.0 / (4.0 * rSq * pow(NdotH, 4));
					double rB = NdotH * NdotH - 1.0;
					double rC = rSq * NdotH * NdotH;

					roughness = rA * exp(rB / rC);
				}
				else if (roughMode == ROUGHMODE_GAUSSIAN)
				{
					double c = 1.0;
					double alpha = acos(iter->normal * halfway);

					roughness = c * exp(-(alpha / rSq));
				}

				double fresnel = pow(1.0 - VdotH, 5.0);
				fresnel *= (1.0 - fresnelTerm);
				fresnel += fresnelTerm;


				double Rs = (fresnel * geo * roughness) / (NdotV * NdotL);

				specular = max(0.0, NdotL) * (iter->light->color * mat.getSpecular() * Rs);
			}
		}

		diffuse *= mat.getDiffuseCoeff() * kd;
		specular *= mat.getSpecularCoeff() * ks;

		return (amb + diffuse + specular);
	}


	double roughValue;
	RoughMode roughMode;
};

#endif