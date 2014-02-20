#ifndef ILLUMINATION_MODEL_H
#define ILLUMINATION_MODEL_H

#include <vector>

#include "intersectdata.h"
#include "material.h"
#include "colour.h"
#include "object3D.h"


/**
 * IlluminationModel - abstract class
 *
 * The base class for all illumination models, such as Phong and CookTorrance.
 * All child classes must implement the illuminate method, which will be used
 * to unique illuminate a scene based on which model is chosen.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class IlluminationModel
{
public:
	IlluminationModel() : ka(0.3), kd(0.6), ks(1), ke(20)
	{
		ambient = new Colour(1, 1, 1);
	}
	IlluminationModel(Colour* amb) : ambient(amb), ka(0.3), kd(0.6), ks(.5), ke(20) {}
	virtual ~IlluminationModel() {}

	/**
	 * illuminate - abstract
	 *
	 * Determines the color of a pixel, using the IntersectData and child
	 * class' implementation of this method
	 *
	 * @param intersects - vector<IntersectData>, 1 for each light source in the scene
	 */
	virtual Colour illuminate(vector<IntersectData> intersects) = 0;

	/*BEGIN: Getters & Setters*/
	const Colour& getAmbient() const
	{
		return *ambient;
	}
	Colour& getAmbient()
	{
		return *ambient;
	}
	void setAmbient(Colour* amb)
	{
		ambient = amb;
	}

	double getAmbientCoeff()
	{
		return ka;
	}
	void setAmbientCoeff(double k)
	{
		ka = k;
	}

	double getDiffuseCoeff()
	{
		return kd;
	}
	void setDiffuseCoeff(double k)
	{
		kd = k;
	}

	double getSpecularCoeff()
	{
		return ks;
	}
	void setSpecularCoeff(double k)
	{
		ks = k;
	}

	double getSpecularSizeCoeff()
	{
		return ke;
	}
	void setSpecularSizeCoeff(double k)
	{
		ke = k;
	}
	/*END: Getters & Setters*/

protected:
	Colour* ambient;
	double ka;
	double kd;
	double ks;
	double ke;
};

#endif