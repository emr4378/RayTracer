#include "sphere.h"

Sphere::Sphere() : Object3D(), radius(0), center()
{
}

Sphere::Sphere(double x, double y, double z, double r, Material* m) : Object3D(m), radius(r), center(x, y, z)
{
}

Sphere::Sphere(double x, double y, double z, double r, Texture* t) : Object3D(t), radius(r), center(x, y, z)
{
}

Sphere::Sphere(double x, double y, double z, double r, Material* m, Texture* t) : Object3D(m, t), radius(r), center(x, y, z)
{
}

Sphere::~Sphere()
{
}

double Sphere::intersect(Ray ray) const
{
	Vector3 vectTo = ray.origin - center;

	double A = ray.direction * ray.direction;
	double B = 2 * (ray.direction * vectTo);
	double C = pow(vectTo.x, 2)
	           + pow(vectTo.y, 2)
	           + pow(vectTo.z, 2)
	           - radius * radius;

	double det = B * B - 4 * C;
	if (det < 0)
	{
		return -1;
	}
	else
	{
		double sqrtDet = sqrt(det);
		double t1 = (-B + sqrtDet) / (2);
		double t2 = (-B - sqrtDet) / (2);

		double t = min(t1, t2);

		return t;
	}
}

IntersectData Sphere::evaluateIntersect(Ray ray, double t) const
{
	Point3 point = ray.origin + ray.direction * t;
	Vector3 normal = point - center;
	normal.normalize();

	IntersectData ret(point, -ray.direction, normal);

	//TODO: Add (u, v) determination here
	ret.material = *material;

	return ret;
}