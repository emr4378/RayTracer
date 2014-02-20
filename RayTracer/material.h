#ifndef MATERIAL_H
#define MATERIAL_H

#include "colour.h"

#define DEFAULT_AMB		0.3
#define DEFAULT_DIFF	0.6
#define DEFAULT_SPEC	0.3
#define DEFAULT_SPEC_S	200

#define DEFAULT_REFL	0
#define DEFAULT_TRANS	0
#define DEFAULT_REFR	0.95

#define DEFAULT_R		1
#define DEFAULT_G		0.5
#define DEFAULT_B		0

#define DEFAULT_S_R		1
#define DEFAULT_S_G		1
#define DEFAULT_S_B		1

/**
 * Material
 *
 * A representation of a material that can be applied to objects. Stores everything
 * that is pertains to how an object will look when rendered
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Material
{
public:
	Material() : diffuse(DEFAULT_R, DEFAULT_G, DEFAULT_B),
		ambient(DEFAULT_R, DEFAULT_G, DEFAULT_B),
		specular(DEFAULT_S_R, DEFAULT_S_G, DEFAULT_S_B),
		ka(DEFAULT_AMB),
		kd(DEFAULT_DIFF),
		ks(DEFAULT_SPEC),
		ke(DEFAULT_SPEC_S),
		kr(DEFAULT_REFL),
		kt(DEFAULT_TRANS),
		n(DEFAULT_REFR) {}

	Material(double r, double g, double b) :
		diffuse(r, g, b),
		ambient(r, g, b),
		specular(DEFAULT_S_R, DEFAULT_S_G, DEFAULT_S_B),
		ka(DEFAULT_AMB),
		kd(DEFAULT_DIFF),
		ks(DEFAULT_SPEC),
		ke(DEFAULT_SPEC_S),
		kr(DEFAULT_REFL),
		kt(DEFAULT_TRANS),
		n(DEFAULT_REFR) {}

	Material(double r, double g, double b,
	         double kamb, double kdiff, double kspec, double kspecS,
	         double kref, double ktrans, double iRefr) :
		diffuse(r, g, b),
		ambient(r, g, b),
		specular(DEFAULT_S_R, DEFAULT_S_G, DEFAULT_S_B),
		ka(kamb),
		kd(kdiff),
		ks(kspec),
		ke(kspecS),
		kr(kref),
		kt(ktrans),
		n(iRefr) {}

	Material(Colour col) :
		diffuse(col),
		ambient(col),
		specular(DEFAULT_S_R, DEFAULT_S_G, DEFAULT_S_B),
		ka(DEFAULT_AMB),
		kd(DEFAULT_DIFF),
		ks(DEFAULT_SPEC),
		ke(DEFAULT_SPEC_S),
		kr(DEFAULT_REFL),
		kt(DEFAULT_TRANS),
		n(DEFAULT_REFR) {}

	Material(Colour col,
	         double kamb, double kdiff, double kspec, double kspecS,
	         double kref, double ktrans, double iRefr) :
		diffuse(col),
		ambient(col),
		specular(DEFAULT_S_R, DEFAULT_S_G, DEFAULT_S_B),
		ka(kamb),
		kd(kdiff),
		ks(kspec),
		ke(kspecS),
		kr(kref),
		kt(ktrans),
		n(iRefr)  {}

	Material(Colour a, Colour d, Colour s,
	         double kamb, double kdiff, double kspec, double kspecS,
	         double kref, double ktrans, double iRefr) :
		diffuse(d),
		ambient(a),
		specular(s),
		ka(kamb),
		kd(kdiff),
		ks(kspec),
		ke(kspecS),
		kr(kref),
		kt(ktrans),
		n(iRefr)  {}

	~Material() {}


	/*BEGIN: Getters & Setters*/
	const Colour& getAmbient() const
	{
		return ambient;
	}
	Colour& getAmbient()
	{
		return ambient;
	}
	void setAmbient(Colour a)
	{
		ambient = a;
	}
	void setAmbient(double r, double g, double b)
	{
		ambient.r = r;
		ambient.g = g;
		ambient.b = b;
	}

	const Colour& getDiffuse() const
	{
		return diffuse;
	}
	Colour& getDiffuse()
	{
		return diffuse;
	}
	void setDiffuse(Colour d)
	{
		diffuse = d;
	}
	void setDiffuse(double r, double g, double b)
	{
		diffuse.r = r;
		diffuse.g = g;
		diffuse.b = b;
	}

	const Colour& getSpecular() const
	{
		return specular;
	}
	Colour& getSpecular()
	{
		return specular;
	}
	void setSpecular(Colour s)
	{
		specular = s;
	}
	void setSpecular(double r, double g, double b)
	{
		specular.r = r;
		specular.g = g;
		specular.b = b;
	}

	double getReflectiveCoeff()
	{
		return kr;
	}
	void setReflectiveCoeff(double k)
	{
		kr = k;
	}

	double getTransmissiveCoeff()
	{
		return kt;
	}
	void setTransmissiveCoeff(double k)
	{
		kt = k;
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

	double getRefractionIndex()
	{
		return n;
	}
	void setRefractionIndex(double k)
	{
		n = k;
	}

	Colour getAverage()
	{
		Colour ret((ambient.r + diffuse.r) / 2,
		           (ambient.g + diffuse.g) / 2,
		           (ambient.b + diffuse.b) / 2);
		return ret;
	}
	/*END: Getters & Setters*/


protected:
	Colour ambient;
	Colour diffuse;
	Colour specular;

	double ka;	//coefficient of ambient light
	double kd;	//coefficient of diffuse light
	double ks;	//coefficient of specular light
	double ke;	//coefficient of specular highlight size/dispersal?
	double kr;	//coefficient of reflection
	double kt;	//coefficient of transmission
	double n;	//index of refraction
};

#endif