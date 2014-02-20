#ifndef COLOUR_H
#define COLOUR_H

#include <iostream>

/**
 * Colour
 *
 * A basic color class representation, inspired by the "vecmath" class. 
 * Has support for numerous operations commonly done
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Colour
{
public:

	Colour() : r(0), g(0), b(0) {}
	Colour(float * c) : r(c[0]), g(c[1]), b(c[2]) {}
	Colour(double * c) : r(c[0]), g(c[1]), b(c[2]) {}
	Colour(float inR, float inG, float inB) : r(inR), g(inG), b(inB) {}
	~Colour() {}

	Colour& operator+=(const Colour& c) {
		r += c.r; g += c.g; b += c.b;
		return *this;
	}
	Colour& operator-=(const Colour& c) {
		r -= c.r; g -= c.g; b -= c.b;
		return *this;
	}
	Colour& operator*=(const Colour& c) {
		r *= c.r; g *= c.g; b *= c.b;
		return *this;
	}

	Colour& operator+=(double s) {
		r += s; g += s; b += s;
		return *this;
	}

	Colour& operator-=(double s) {
		r -= s; g -= s; b -= s;
		return *this;
	}

	Colour& operator*=(double s) {
		r *= s; g *= s; b *= s;
		return *this;
	}

	float getLuminance() {
		return (0.27 * r + 0.67 * g + 0.06 * b);
	}

	float r;
	float g;
	float b;
};

inline Colour operator+(const Colour& a, double s) {
  return Colour(a.r + s, a.g + s, a.b + s);
}
inline Colour operator+(double s, const Colour& a) {
  return Colour(a.r + s, a.g + s, a.b + s);
}
inline Colour operator+(const Colour& a, const Colour& b) {
	return Colour(a.r + b.r, a.g + b.g, a.b + b.b);
}

inline Colour operator-(const Colour& a, double s) {
  return Colour(a.r - s, a.g - s, a.b - s);
}
inline Colour operator-(double s, const Colour& a) {
  return Colour(a.r - s, a.g - s, a.b - s);
}
inline Colour operator-(const Colour& a, const Colour& b) {
	return Colour(a.r - b.r, a.g - b.g, a.b - b.b);
}

inline Colour operator*(const Colour& a, double s) {
  return Colour(a.r * s, a.g * s, a.b * s);
}
inline Colour operator*(double s, const Colour& a) {
  return Colour(a.r * s, a.g * s, a.b * s);
}
inline Colour operator*(const Colour& a, const Colour& b) {
	return Colour(a.r * b.r, a.g * b.g, a.b * b.b);
}

inline std::ostream& operator<<(std::ostream& os, const Colour& c) {
    os << "Colour(" << c.r << "," << c.g << "," << c.b << ")";
	return os;
}

#endif