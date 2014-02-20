#ifndef CYLINDER_H
#define CYLINDER_H

#include "object3d.h"

//TODO: Fix evaluateIntersect; normal calculation is wrong

/**
 * Cylinder
 *
 * Represents a non-axis-aligned cylinder of variable size.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Cylinder :
	public Object3D
{
public:
	Cylinder();
	Cylinder(double x, double y, double z,
	         double aX, double aY, double aZ,
	         double r, double minZ, double maxZ,
	         Material* m, Texture* t);

	virtual ~Cylinder();

	double intersect(Ray ray) const;
	IntersectData evaluateIntersect(Ray ray, double t) const;

	Point3 center;
	Vector3 axis;
	double zMin;
	double zMax;
	double radius;
};

#endif