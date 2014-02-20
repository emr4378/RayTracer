#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "intersectdata.h"
#include "material.h"
#include "texture.h"

/**
 * Object3D - abstract class
 *
 * A representation of a generic object in 3 dimensions. Will be the base class
 * of every 3d object this system can support. Stores the information that all
 * objects will have, such as Material, Texture, and which to use
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Object3D
{

public:
	Object3D() : material(0), texture(0) {}
	Object3D(Material* m) : material(m), texture(0) {}
	Object3D(Texture* t) : material(0), texture(t), useTexture(true) {}
	Object3D(Material* m, Texture* t) : material(m), texture(t)
	{
		if (t != 0)
		{
			useTexture = true;
		}
	}
	virtual ~Object3D()
	{
		delete material;
		delete texture;
	}

	/**
	 * intersect
	 *
	 * Determines what time, if any, the given ray has intersected with this object
	 *
	 * @param ray - Ray, from the camera outwards, to be checked against
	 * @return double - time of intersection of there is one, -1 otherwise
	 */
	virtual double intersect(Ray ray) const = 0;

	/**
	 * evaluateIntersect
	 *
	 * Using the given Ray and time, determines at what point an intersection will occur
	 * and generates the relevant IntersectData for it (with the base information; point
	 * of intersection, direction to camera, and surface normal.
	 *
	 * Also where (u, v) coordinate determination is done
	 *
	 * @param ray - Ray, the ray being cast towards the object
	 * @param t - double, the time of intersection
	 * @param return IntersectData, with valid point, direction, and normal
	 */
	virtual IntersectData evaluateIntersect(Ray ray, double t) const = 0;


	/*BEGIN: Getters & Setters*/
	Material const* getMaterial() const
	{
		return material;
	}
	Material* getMaterial()
	{
		return material;
	}
	void setMaterial(Material* m)
	{
		delete material;
		material = m;
	}

	Texture const* getTexture() const
	{
		return texture;
	}
	Texture* getTexture()
	{
		return texture;
	}
	void setTexture(Texture* t)
	{
		delete texture;
		texture = t;
	}

	bool getTextureUse()
	{
		return useTexture;
	}
	void setTextureUse(bool u)
	{
		useTexture = u;
	}
	/*END: Getters & Setters*/

protected:
	Material* material;
	Texture* texture;
	bool useTexture;
};

#endif