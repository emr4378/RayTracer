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
	Cylinder(float x, float y, float z,
			float aX, float aY, float aZ,
			float r, float minZ, float maxZ,
			Material * m, Texture * t);

	virtual ~Cylinder();

	float intersect(Ray ray) const;
	IntersectData evaluateIntersect(Ray ray, float t) const;

	Point3 center;
	Vector3 axis;
	float zMin;
	float zMax;
	float radius;
};

#endif