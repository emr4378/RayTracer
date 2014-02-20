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
	enum RoughMode { BECKMANN, GAUSSIAN };

	CookTorrance() : IlluminationModel(), roughValue(0.5f), roughMode(RoughMode::BECKMANN) {}

	CookTorrance(float rVal, RoughMode rMode) : IlluminationModel(), roughValue(rVal), roughMode(rMode) {}

	CookTorrance(Colour * amb) : IlluminationModel(amb) {}

	~CookTorrance() {}

	Colour illuminate(vector<IntersectData> intersects) {
		Material& mat = intersects.front().material;

		Colour amb = (mat.getAmbient() * (*ambient)) * mat.getAmbientCoeff() * ka;
		Colour diffuse;
		Colour specular;

		float fresnelTerm = intersects.front().direction * intersects.front().normal;

		vector<IntersectData>::iterator iter;
		for (iter = intersects.begin(); iter != intersects.end(); iter++) {
			if (iter->light != 0) {
				diffuse += iter->light->color * mat.getDiffuse() * (iter->incoming * iter->normal);


				Vector3 halfway = iter->direction + iter->incoming;
				halfway.normalize();

				float NdotL	= iter->normal * iter->incoming;
				float NdotH = iter->normal * halfway;
				float NdotV = iter->normal * iter->direction;
				float VdotH = iter->direction * halfway;
				float rSq = roughValue * roughValue;


				float temp1 = (2 * NdotH * NdotV) / VdotH;
				float temp2 = (2 * NdotH * NdotL) / VdotH;
				float geo = min(1.0f, min(temp1, temp2));

				float roughness;

				if (roughMode == RoughMode::BECKMANN) {
					float rA = 1.0f / (4.0f * rSq * pow(NdotH, 4));
					float rB = NdotH * NdotH - 1.0f;
					float rC = rSq * NdotH * NdotH;

					roughness = rA * exp(rB / rC);
				} else if (roughMode == RoughMode::GAUSSIAN) {
					float c = 1.0f;
					float alpha = acos(iter->normal * halfway);

					roughness = c * exp( -(alpha / rSq) );
				}

				float fresnel = pow(1.0f - VdotH, 5.0f);
				fresnel *= (1.0f - fresnelTerm);
				fresnel += fresnelTerm;


				float Rs = (fresnel * geo * roughness) / (NdotV * NdotL);

				specular = max(0.0f, NdotL) * (iter->light->color * mat.getSpecular() * Rs);
			}
		}

		diffuse *= mat.getDiffuseCoeff() * kd;
		specular *= mat.getSpecularCoeff() * ks;

		return (amb + diffuse + specular);
	}


	float roughValue;
	RoughMode roughMode;
};

#endif