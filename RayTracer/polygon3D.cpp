#include "polygon3D.h"

Polygon3D::Polygon3D() : Object3D(), vertices(), normal() {
}

Polygon3D::Polygon3D(vector<Point3> verts, Vector3 n, Material * m) : Object3D(m), vertices(verts), normal(n) {
	normal.normalize();

	origin = verts[0];
	int imin = 0;
	for (int i = 0; i < 3; i++) {
		if (abs(normal[i]) < abs(normal[imin])) {
			imin = i;
		}
	}

	float dt = normal[imin];
	imin == 0 ? right.x = 1 : (imin == 1 ? right.y = 1 : right.z = 1);
	
	right.x -= dt * normal.x;
	right.y -= dt * normal.y;
	right.z -= dt * normal.z;

	up = normal ^ right;

	right.normalize();
	up.normalize();
}

Polygon3D::Polygon3D(vector<Point3> verts, Vector3 n, Texture * t) : Object3D(t), vertices(verts), normal(n) {
	normal.normalize();

	origin = verts[0];
	int imin = 0;
	for (int i = 0; i < 3; i++) {
		if (abs(normal[i]) < abs(normal[imin])) {
			imin = i;
		}
	}

	float dt = normal[imin];
	imin == 0 ? right.x = 1 : (imin == 1 ? right.y = 1 : right.z = 1);
	
	right.x -= dt * normal.x;
	right.y -= dt * normal.y;
	right.z -= dt * normal.z;

	up = normal ^ right;
		right.normalize();
	up.normalize();
}

Polygon3D::Polygon3D(vector<Point3> verts, Vector3 n, Material * m, Texture * t) : Object3D(m, t), vertices(verts), normal(n) {
	normal.normalize();

	origin = verts[0];
	int imin = 0;
	for (int i = 0; i < 3; i++) {
		if (abs(normal[i]) < abs(normal[imin])) {
			imin = i;
		}
	}

	float dt = normal[imin];
	imin == 0 ? right.x = 1 : (imin == 1 ? right.y = 1 : right.z = 1);
	
	right.x -= dt * normal.x;
	right.y -= dt * normal.y;
	right.z -= dt * normal.z;

	up = normal ^ right;
		right.normalize();
	up.normalize();
}

Polygon3D::~Polygon3D() {
}


float Polygon3D::intersect(Ray ray) const {
	float VD = normal * ray.direction;
	if (VD != 0) {
		Vector3 aPoint(vertices[0].x, vertices[0].y, vertices[0].z);

		float F = -(  normal.x * vertices[0].x
					+ normal.y * vertices[0].y
					+ normal.z * vertices[0].z);
		float t = - ( normal.x * ray.origin.x
					+ normal.y * ray.origin.y
					+ normal.z * ray.origin.z
					+ F) / (VD);

		if (t > 0) {
			Point3 intersect = ray.origin + ray.direction * t;

			float thetaSum = 0;
			float ang;
			Vector3 vectTo = vertices.back() - intersect;
			Vector3 tempV;

			vector<Point3>::const_iterator iter;
			for (iter = vertices.begin(); iter != vertices.end(); iter++) {
				tempV = *iter - intersect;

				vectTo.normalize();
				tempV.normalize();

				ang = tempV * vectTo;
				ang = acos(ang);
				thetaSum += ang;
				vectTo = tempV;
			}

			if (fabs(thetaSum - (2 * PI)) >= .1) {
				return -1;
			} else {
				return t;
			}
		} else {
			return -1;
		}
	} else {
		return -1;
	}
}

IntersectData Polygon3D::evaluateIntersect(Ray ray, float t) const {
	IntersectData ret((ray.origin + ray.direction * t), -ray.direction, normal);
	
	if (useTexture && texture != 0) {

		Vector3 temp = Vector3(	ret.point.x - origin.x, 
								ret.point.y - origin.y, 
								ret.point.z - origin.z);

		int u = right * temp - 1;
		int v = up * temp;

		if (material != 0) {
			ret.material = Material((*material));
			ret.material.setAmbient(texture->getColour(u, v));
			ret.material.setDiffuse(texture->getColour(u, v));
		} else {
			ret.material = Material(texture->getColour(u, v));
		}
	} else {
		ret.material = *material;
	}

	return ret;
}