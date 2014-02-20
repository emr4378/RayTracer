#ifndef PLANE_H
#define PLANE_H

#include "object3d.h"

/**
 * Plane
 *
 * Represents an infinite plane in 3D space, using a point and a surface normal
 * Also stores relative up and right axis, used for determining (u, v) coodinates
 * on this object
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Plane :
	public Object3D
{
public:
	Plane();
	Plane(Point3 aPoint, Vector3 n, Material * m, Texture * t);

	virtual ~Plane();

	float intersect(Ray ray) const;
	IntersectData evaluateIntersect(Ray ray, float t) const;

	Vector3 normal;
	Vector3 up;
	Vector3 right;
	Point3 origin;
};


#endif