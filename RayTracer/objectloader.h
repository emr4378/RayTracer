#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include <vector>

#include "world.h"
#include "camera.h"

#include "object3D.h"
#include "plane.h"
#include "polygon3D.h"
#include "sphere.h"
#include "lightsource.h"
#include "material.h"

#include "Libraries\objTester\objLoader.h"

/**
 * ObjectLoader
 *
 * A wrapper class for the kixor .obj loader library. Converts the kixor data structures into
 * this projects equivalent.
 *
 * @author Eduardo Rodrigues
 */
class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();

	
	void load(char * filename, World * world, Camera * cam);

private:
	/**
	 * convert2P
	 * Converts the kixor object "obj_vector" into it's equivalent Point3;
	 */
	Point3 convert2P(obj_vector * v);

	/**
	 * convert2V
	 * Converts the kixor object "obj_vector" into it's equivalent
	 */
	Vector3 convert2V(obj_vector * v);

	/**
	 * convert2M
	 * Converts the kixor object "obj_material" into it's equivalent
	 */
	Material * convert2M(obj_material * m);

	objLoader * objData;	//the kixor object loader to be used
};

#endif