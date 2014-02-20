#ifndef RAY_H
#define RAY_H

#include "vecmath.h"

/**
 * Ray*
 *
 * A basic representation of a ray; a point with direction

 * @author Eduardo Rodrigues
 */
class Ray
{
public:
	Ray() : origin(), direction() {}
	Ray(const Point3 &o, const Vector3 &v) : origin(o), direction(v) {
		direction.normalize();
	}

	Point3 origin;
	Vector3 direction;
};

inline std::ostream& operator<<(std::ostream& os, const Ray& r) {
    os << "Ray At:" << r.origin << " Dir: " << r.direction;
	return os;
}

#endif