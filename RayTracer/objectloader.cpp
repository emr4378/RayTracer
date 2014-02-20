#include "objectloader.h"


ObjectLoader::ObjectLoader() {
	objData = new objLoader();
}


ObjectLoader::~ObjectLoader() {
	//delete objData;
}


Point3 ObjectLoader::convert2P(obj_vector * v) {
	Point3 ret;
	if (v != 0) {
		ret.x = v->e[0];
		ret.y = v->e[1];
		ret.z = v->e[2];
	}
	return ret;
}

Vector3 ObjectLoader::convert2V(obj_vector * v) {
	Vector3 ret;
	if (v != 0) {
		ret.x = v->e[0];
		ret.y = v->e[1];
		ret.z = v->e[2];
	}
	return ret;
}

Material * ObjectLoader::convert2M(obj_material * m) {
	Material * mat = new Material();
	mat->setReflectiveCoeff(0);
	mat->setTransmissiveCoeff(0);
	mat->setRefractionIndex(0);

	if (m != 0) {
	/*	mat->setAmbientCoeff(m->amb[0]);
		mat->setDiffuseCoeff(m->diff[0]);
		mat->setSpecularCoeff(m->spec[0]);*/
		m->amb != 0 ? mat->setAmbient(Colour(m->amb)) : 0;
		m->diff != 0 ? mat->setDiffuse(Colour(m->diff)) : 0;
		m ->spec != 0 ? mat->setSpecular(Colour(m->spec)) : 0;
		mat->setReflectiveCoeff(m->reflect);
		mat->setTransmissiveCoeff(m->trans);
		mat->setSpecularSizeCoeff(m->shiny);
		mat->setRefractionIndex(m->refract_index);
	}


	return mat;
}

void ObjectLoader::load(char * filename, World * world, Camera * cam) {
	objData->load(filename);

	//camera
	if (cam != 0 && objData->camera != 0) {
		obj_camera *o = objData->camera;
		obj_vector *p = o->camera_pos_index != -1 ? objData->vertexList[ o->camera_pos_index ] : 0;
		obj_vector *l = o->camera_look_point_index != -1 ? objData->vertexList[ o->camera_look_point_index ] : 0;
		obj_vector *u = o->camera_up_norm_index != -1 ? objData->vertexList[ o->camera_up_norm_index ] : 0;

		cam->lookAt(convert2P(p), convert2P(l), convert2V(u));
	}

	if (world == 0) {
		return;
	}

	//triangle/polygon faces
	for(int i=0; i<objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];
		obj_vector *p;
		obj_material *m = o->material_index != -1 ? objData->materialList[ o->material_index ] : 0;

		//TODO: Vertex normal support (store each normal, LERP betwen them)
		//TODO: Texture support (first figure out texture coordinates)

		vector<Point3> points;
		for (int j = 0; j < o->vertex_count; j++) {
			p = objData->vertexList[ o->vertex_index[j] ];
			points.push_back(convert2P(p));
		}

		//Vector3& norm = (points[1] - points[0]) ^ (points[2] - points[0]);
		Vector3& norm = (points[1] - points[0]) ^ (points[2] - points[0]);
		norm.normalize();
		Material * mat = convert2M(m);

		world->add( new Polygon3D(points, norm, mat) );

		//add( new Polygon3D(points, convert2V(n), convert2M(m)) );
	}

	//spheres
	for(int i=0; i<objData->sphereCount; i++)
	{
		obj_sphere *o = objData->sphereList[i];
		obj_vector *p = o->pos_index != -1 ? objData->vertexList[ o->pos_index ] : 0;
		obj_vector *n = o->up_normal_index != -1 ? objData->vertexList[ o->up_normal_index ] : 0;
		obj_material *m = o->material_index != -1 ? objData->materialList[ o->material_index ] : 0;

		//TODO: Texture support (first figure out texture coordinates)
		float x = 0;
		float y = 0;
		float z = 0;
		if (p != 0) {
			x = p->e[0];
			y = p->e[1];
			z = p->e[2];
		}

		world->add( new Sphere(x, y, z, convert2V(n).length(), convert2M(m)) );
	}

	//planes
	for(int i=0; i<objData->planeCount; i++)
	{
		obj_plane *o = objData->planeList[i];
		obj_vector *p = o->pos_index != -1 ? objData->vertexList[ o->pos_index ] : 0;
		obj_vector *n = o->normal_index != -1 ? objData->vertexList[ o->normal_index ] : 0;
		obj_material *m = o->material_index != -1 ? objData->materialList[ o->material_index ] : 0;

		//TODO: Rotation normal support
		//TODO: Texture support (first figure out texture coordinates)

		world->add( new Plane(convert2P(p), convert2V(n), convert2M(m), 0) ); 
	}

	//point lights
	for(int i=0; i<objData->lightPointCount; i++)
	{
		obj_light_point *o = objData->lightPointList[i];
		obj_vector *p = o->pos_index != -1 ? objData->vertexList[ o->pos_index ] : 0;
		obj_material *m = o->material_index != -1 ? objData->materialList[ o->material_index ] : 0;

		//TODO: Possibly better color support (would require overhaul of LightSource)

		world->add( new LightSource(convert2P(p), Colour(m->amb)) );
	}

	for(int i = 0; i < objData->lightQuadCount; i++) {
		obj_light_quad *o = objData->lightQuadList[i];
		obj_material *m = o->material_index != -1 ? objData->materialList[ o->material_index ] : 0;

		Point3 total;
		for (int j = 0; j < 4; j++) {
			total += convert2V(objData->vertexList[ o->vertex_index[j] ]);
		}
		total *= 1.0f/4.0f;

		//TODO: Possibly better color support (would require overhaul of LightSource)

		world->add( new LightSource(total, Colour(m->amb)) );
	}
}