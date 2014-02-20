#ifndef POLYGON3D_H
#define POLYGON3D_H

#include <vector>

#include "object3d.h"
#include "vecmath.h"

/**
 * Polygon3D
 *
 * Represents a 3D polygon; essentially a range-restricted Plane. 
 * Should always contain more than 3 points, but is not enforced.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Polygon3D :
	public Object3D
{
public:
	Polygon3D();
	Polygon3D(vector<Point3> verts, Vector3 n, Material * m);
	Polygon3D(vector<Point3> verts, Vector3 n, Texture * t);
	Polygon3D(vector<Point3> verts, Vector3 n, Material * m, Texture * t);

	virtual ~Polygon3D();

	float intersect(Ray ray) const;
	IntersectData evaluateIntersect(Ray ray, float t) const;

	vector<Point3> vertices;
	Vector3 normal;
	Vector3 up;
	Vector3 right;
	Point3 origin;
};

#endif