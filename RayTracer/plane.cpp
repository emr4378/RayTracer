#include "plane.h"

Plane::Plane() : Object3D(), origin(0, 0, 0), up(0, 0, 1), right(1, 0, 0), normal(0, 1, 0)
{
}

Plane::Plane(Point3 aPoint, Vector3 n, Material* m, Texture* t) : Object3D(m, t), origin(aPoint), normal(n)
{
	normal.normalize();

	int imin = 0;
	for (int i = 0; i < 3; i++)
	{
		if (abs(normal[i]) < abs(normal[imin]))
		{
			imin = i;
		}
	}

	double dt = normal[imin];
	imin == 0 ? right.x = 1 : (imin == 1 ? right.y = 1 : right.z = 1);

	right.x -= dt * normal.x;
	right.y -= dt * normal.y;
	right.z -= dt * normal.z;

	up = normal ^ right;

	right.normalize();
	up.normalize();
}

Plane::~Plane()
{
}


double Plane::intersect(Ray ray) const
{
	double t = -(normal * (ray.origin - origin)) / (normal * ray.direction);

	return t;
}

IntersectData Plane::evaluateIntersect(Ray ray, double t) const
{
	IntersectData ret((ray.origin + ray.direction * t), -ray.direction, normal);

	if (useTexture && texture != 0)
	{
		Vector3 temp = Vector3(ret.point.x - origin.x,
		                       ret.point.y - origin.y,
		                       ret.point.z - origin.z);

		int u = (int)(right * temp - 1);
		int v = (int)(up * temp);

		Colour col;
		texture->getColour(u, v, col);
		ret.material = Material(col);
	}
	else
	{
		ret.material = *material;
	}

	return ret;
}