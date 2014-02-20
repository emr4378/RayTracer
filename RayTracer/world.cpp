#include "world.h"


World::World() : objectList(), lightList()
{
}


World::~World()
{
	//vector<Object3D *>::iterator iter;
	//for (iter = objectList.begin(); iter != objectList.end(); iter++) {
	//	Object3D * val = *iter;
	//	delete val;
	//}
}

void World::add(Object3D * obj) {
	objectList.push_back(obj);
}

void World::remove(Object3D * obj) {
	objectList.erase( find(objectList.begin(), objectList.end(), obj) );
}

void World::add(LightSource * light) {
	lightList.push_back(light);
}

void World::remove(LightSource * light) {
	lightList.erase( find(lightList.begin(), lightList.end(), light) );
}

void World::transformAllObjects(float** m) {
}