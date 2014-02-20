#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <algorithm>

#include "Object3D.h"
#include "lightsource.h"

/**
 * World
 *
 * Represents the geometry of the scene; stores both light sources and scene models
 * and provides convenience methods for adding/removing thost objects
 *
 * @author Eduardo Rodrigues - emr4378
 */
class World
{
public:
	World();
	~World();

	void add(Object3D * obj);
	void remove(Object3D * obj);
	void add(LightSource * light);
	void remove(LightSource * light);
	void transformAllObjects(float** m);

	vector<Object3D *> objectList;
	vector<LightSource *> lightList;

	//TODO: Attributes List
};

#endif