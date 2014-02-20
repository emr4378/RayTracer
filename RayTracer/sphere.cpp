#include "sphere.h"

Sphere::Sphere() : Object3D(), radius(0), center() {
}

Sphere::Sphere(float x, float y, float z, float r, Material * m) : Object3D(m), radius(r), center(x, y, z) {
}

Sphere::Sphere(float x, float y, float z, float r, Texture * t) : Object3D(t), radius(r), center(x, y, z) {
}

Sphere::Sphere(float x, float y, float z, float r, Material * m, Texture * t) : Object3D(m, t), radius(r), center(x, y, z) {
}

Sphere::~Sphere() {
}

float Sphere::intersect(Ray ray) const {
	Vector3 vectTo = ray.origin - center;

	float A = ray.direction * ray.direction;
	float B = 2 * (ray.direction * vectTo);
	float C = pow(vectTo.x, 2)
			+ pow(vectTo.y, 2)
			+ pow(vectTo.z, 2)
			- radius * radius;

	float det = B * B - 4 * C;
	if (det < 0) {
		return -1;
	} else {
		float sqrtDet = sqrt(det);
		float t1 = (-B + sqrtDet) / (2);
		float t2 = (-B - sqrtDet) / (2);

		float t = min(t1, t2);

		return t;
	}
}

IntersectData Sphere::evaluateIntersect(Ray ray, float t) const {
	Point3 point = ray.origin + ray.direction * t;
	Vector3 normal = point - center;
	normal.normalize();

	IntersectData ret(point, -ray.direction, normal);

	//TODO: Add (u, v) determination here
	ret.material = *material;

	return ret;
}