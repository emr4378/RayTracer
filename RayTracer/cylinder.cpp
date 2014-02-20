#include "cylinder.h"


Cylinder::Cylinder() : Object3D(), 
						center(), axis(0, 1, 0), 
						radius(1), zMin(-1), zMax(1){}

Cylinder::Cylinder(	float x, float y, float z, 
					float aX, float aY, float aZ, 
					float r, float minZ, float maxZ,
					Material * m, Texture * t) :Object3D(m, t), 
												center(x, y, z), 
												axis(aX, aY, aZ), 
												radius(r), 
												zMin(minZ), 
												zMax(maxZ) {
													axis.normalize();
}


Cylinder::~Cylinder() {}

float Cylinder::intersect(Ray ray) const {
	Vector3 up(0, 1, 0);
	float angle = acos(axis * up) * 180 / PI;

	Vector3& Co = Vector3(center.x, center.y, center.z);
	Vector3& Ro = Vector3(ray.origin.x, ray.origin.y, ray.origin.z);
	Vector3& Rd = ray.direction;

	if (angle * angle > 0.01f) {
		Vector3 rotAxis = axis ^ up;
		Co = Vector3::rotate(Co, rotAxis, -angle);
		Ro = Vector3::rotate(Ro, rotAxis, -angle);
		Rd = Vector3::rotate(Rd, rotAxis, -angle);
	} 	
	Ro -= Co;
	
	float A = Rd.x * Rd.x + Rd.z * Rd.z;
	float B = 2 * (Ro.x * Rd.x + Ro.z * Rd.z);
	float C = Ro.x * Ro.x + Ro.z * Ro.z - radius * radius;

	float det = B * B - 4 * A * C;
	if (det >= 0) {
		float sqrtDet = sqrt(det);
		float t1 = (-B + sqrtDet) / (2 * A);
		float t2 = (-B - sqrtDet) / (2 * A);

		float t = min(t1, t2);

		float yVal = Ro.y + Rd.y * t;

		if (yVal > zMin && yVal < zMax) {
			return t;
		}
	}

	if (Rd.y != 0) {
		float t1 = -(Ro.y - zMax) / Rd.y;
		float t2 = -(Ro.y - zMin) / Rd.y;

		float t = min(t1, t2);

		if(t > 0) {
			Vector3 poi = (Ro + Rd * t);
			if(poi.x * poi.x + poi.z * poi.z <= radius * radius) {
				return t;
			}
		}
	}

	return -1;
}

IntersectData Cylinder::evaluateIntersect(Ray ray, float t) const {
	Vector3 up(0, 1, 0);
	float angle = acos(axis * up) * 180 / PI;


	Point3 point = ray.origin + ray.direction * t;

	Point3 Co = center;
	Point3 Po = point;

	if (angle * angle > 0.01f) {
		Vector3 rotAxis = axis ^ up;
		Po = Point3::rotate(Po, rotAxis, -angle);
		Co = Point3::rotate(Co, rotAxis, -angle);
	}
	Po.x -= Co.x;
	Po.y -= Co.y;
	Po.z -= Co.z;


	Vector3 normal;
	if (Po.y == zMax) {
		normal = axis;
	} else if (Po.y == zMin) {
		normal = -axis;
	} else {
		normal = point - center;
		normal = normal - (normal * axis) * axis;
		normal.normalize();
	}

	IntersectData ret(point, -ray.direction, -normal);

	//TODO: Add (u, v) determination here
	ret.material = *material;

	return ret;
}