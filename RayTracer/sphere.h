#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.h"

/**
 * Sphere
 *
 * Represents a sphere in 3d space. An all around awesome object, it's easy to
 * store and its intersect and evaluateIntersect methods are quick and easy
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Sphere :
	public Object3D
{
public:
	Sphere();
	Sphere(double x, double y, double z, double r, Material* m);
	Sphere(double x, double y, double z, double r, Texture* t);
	Sphere(double x, double y, double z, double r, Material* m, Texture* t);

	virtual ~Sphere();

	double intersect(Ray ray) const;
	IntersectData evaluateIntersect(Ray ray, double t) const;

	Point3 center;
	double radius;
};

#endif