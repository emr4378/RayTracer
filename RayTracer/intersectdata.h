#ifndef INTERSECT_DATA_H
#define INTERSECT_DATA_H

#include "lightsource.h"
#include "vecmath.h"
#include "ray.h"
#include "material.h"


/**
 * IntersectData
 *
 * A basic container class used to pass information about ray intersections around.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class IntersectData
{
public:
	IntersectData() : point(), direction(), normal(), light(0) {}
	IntersectData(Point3 p, Vector3 d, Vector3 n) : point(p), direction(d), normal(n), light(0) {}
	~IntersectData() {}

	Point3	point;		//point of intersection
	Vector3 direction;	//point of intersection to camera
	Vector3 normal;		//surface normal at point
	Vector3 incoming;	//light to point
	Vector3 reflection;	//reflection of incoming
	LightSource* light;
	Material material;
};

#endif