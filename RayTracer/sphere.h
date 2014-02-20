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
	Sphere(float x, float y, float z, float r, Material * m);
	Sphere(float x, float y, float z, float r, Texture * t);
	Sphere(float x, float y, float z, float r, Material * m, Texture * t);

	virtual ~Sphere();

	float intersect(Ray ray) const;
	IntersectData evaluateIntersect(Ray ray, float t) const;

	Point3 center;
	float radius;
};

#endif