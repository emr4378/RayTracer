/**
 * raytracer
 *
 * An okay implementation of a Ray Tracer. Renders a 3d scene onto a 2d image
 * plane. Does so by casting a number of rays out from the camera position
 * and checking for collisions.
 *
 * Is set up to be extensible, allowing for easy addition of additional object
 * types, illumination models, and tone reproducers.
 *
 * Was also designed with expectation that it would eventually be run in real-
 * time; Camera is set up with many features in the OpenGL/Glut camera system,
 * to allow for quick and easy rotation and moving around. Unfortunately it
 * seems this will never be tested.
 *
 * This class is simply provides a main method to test the raytracer; it should
 * contain nothing that the ray tracer is dependent on. All the code for actually
 * drawing the generated image frame is in here.
 *
 * Also, since this project was checkpoint based, there's a checkpoint method;
 * simply give it the checkpoint number you want to see and it should render it.
 *
 * If you want to play change around some stuff, check out
 *
 * @author Eduardo Rodrigues - emr4378
 */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#	include <stdlib.h>
#endif

#include <glut.h>
#include "vecmath.h"
#include "world.h"
#include "camera.h"
#include "sphere.h"
#include "colour.h"
#include "polygon3D.h"
#include "phong.h"
#include "blinnphong.h"
#include "cooktorrance.h"
#include "flat.h"
#include "constant.h"
#include "material.h"
#include "lightsource.h"
#include "reinhardtone.h"
#include "wardtone.h"
#include "cylinder.h"
#include "plane.h"

#include "objectloader.h"

#include "Libraries\objTester\objLoader.h"

#define STRETCH		1		//STRETCH == 0 ? resize => re-render : resize => stretch

double pixSize = 1;			//the size of pixels in the resulting image. Awesome for testing purposes.
bool superSample = false;	//enable/disable super-sampling
double fov = 60.0;			//the field of view of the camera

World world;
Camera* camera;
double wid, hei;
double origWid = -1;
double origHei = -1;
double dist = 2;
double aspect;
Colour* arr;

/**
 * A method to draw a pixel to the screen, given pixel size and such */
void drawPixel(int x, int y, double xSize, double ySize, Colour* col)
{
	glColor3f(col->r, col->g, col->b);
	double xi = xSize / 2;
	double yi = ySize / 2;
	glRecti(x * xSize - xi, y * ySize - yi, x * xSize + xi, y * ySize + yi);
}

/**
 * Hey! This is where I actually draw stuff to the screen. Check out how I
 * stretch on resize; awesomeness.
 */
void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	double xInc, yInc;
	int rows, cols;
	if (STRETCH == 0)
	{
		xInc = yInc = pixSize;
		rows = ceil(hei / pixSize);
		cols = ceil(wid / pixSize);
		camera->setPerspective(fov, aspect, dist, pixSize);
		arr = camera->render(wid, hei);
	}
	else
	{
		xInc = wid * pixSize / origWid;
		yInc = hei * pixSize / origHei;
		rows = ceil(origHei / pixSize);
		cols = ceil(origWid / pixSize);
		if (arr == 0)
		{
			camera->setPerspective(fov, aspect, dist, pixSize);
			arr = camera->render(wid, hei);
		}
	}

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			Colour* c = &arr[y * cols + x];
			drawPixel(x, y, xInc, yInc, c);
		}
	}

	glFlush();
	glutSwapBuffers();
}

/**
 * Da checkpoint function. Easily allows rendering of different checkpoints
 */
void checkpoint(int n, bool useWard, double lMax, double ldMax)
{
	Sphere* _small;
	Sphere* _big;
	Polygon3D* _floor;

	Material* sMat = new Material(.14, .48, .48);
	sMat->setAmbientCoeff(1);
	_small = new Sphere(-4, -5.0 / 3.125, -18, 3.125, sMat);

	Material* bMat = new Material(0, 1, 0);
	bMat->setAmbientCoeff(1);
	_big = new Sphere(0, 0, -13, 3.125, bMat);

	vector<Point3> verts;
	verts.push_back(Point3(7, -5, -7));
	verts.push_back(Point3(-13, -5, -7));
	verts.push_back(Point3(-13, -5, -40));
	verts.push_back(Point3(7, -5, -40));

	Material* fMat = new Material(0, 0, 1);
	fMat->setAmbientCoeff(1);
	_floor = new Polygon3D(verts, Vector3(0, 1, 0), fMat);

	world.add(_big);
	world.add(_small);
	world.add(_floor);

	if (n >= 3)
	{
		camera->enableShadows();
		camera->setIlluminationModel(new Phong());

		vector<Object3D*>::iterator iter;
		for (iter = world.objectList.begin(); iter != world.objectList.end(); iter++)
		{
			(*iter)->getMaterial()->setAmbientCoeff(0.3);
		}

		world.add(new LightSource(Point3(0, 10, -5), Colour(1, 1, 1)));
	}
	if (n >= 4)
	{
		Colour texColors[] = {Colour(1, 0, 0), Colour(1, 1, 0)};

		_floor->setTexture(Texture::generateTiles(4, 4, 2, 2, texColors));
		_floor->setTextureUse(true);
	}

	if (n >= 5)
	{
		camera->setRecurseDepth(5);
		_big->setMaterial(new Material(_big->getMaterial()->getAmbient(),
		                               .15, .25, 1, 20,
		                               .75, 0, .95));
	}

	if (n >= 6)
	{
		camera->setBGColor(0.04, 0.64, 0.8);

		_big->setMaterial(new Material(1, 1, 1,
		                               .075, .075, .2, 20,
		                               .01, .85, .95));

		Material* mat = _small->getMaterial();

		mat->setAmbient(.7, .7, .7);
		mat->setDiffuse(.7, .7, .7);
		mat->setAmbientCoeff(.15);
		mat->setDiffuseCoeff(.25);
		mat->setSpecularCoeff(1);
		mat->setSpecularSizeCoeff(20);
		mat->setReflectiveCoeff(.75);
		mat->setTransmissiveCoeff(0);
		mat->setRefractionIndex(.95);
	}

	if (n >= 7)
	{
		Tone* tone;
		if (useWard)
		{
			tone = new WardTone(lMax, ldMax);
		}
		else
		{
			tone = new ReinhardTone(lMax, ldMax);
		}

		camera->setTone(tone);
	}
}

/**
 * 3D model loading test. Works okay I guess
 */
void cornellBox()
{
	camera->setOrientation(0, 1, 2,
	                       0, 0, -1,
	                       0, 1, 0);
	camera->enableShadows();
	camera->setIlluminationModel(new Phong());
	camera->setRecurseDepth(0);
	camera->setBGColor(0.04, 0.64, 0.8);

	ObjectLoader loader;
	loader.load("CornellBox-Original.obj", &world, camera);
}

//////////////////////////////////////////////////////////////
//WOAH. Hey now, this is where you'll want to mess around.	//
//															//
//Basically you interact with the camera and the world.		//
//															//
//Create objects as you please, then use world->add() so	//
//that the objects are placed in the scene.					//
//////////////////////////////////////////////////////////////
void init(void)
{
	fov = 60;
	pixSize = 1;
	superSample = false;

	world = World();
	camera = new Camera();
	camera->setWorld(world);
	//camera->lookAt(-2, 0, 0, -2, 0, -13, 0, 1, 0);
	camera->setOrientation(-2, 0, 0,
	                       0, 0, -1,
	                       0, 1, 0);


	checkpoint(7, true, 100, 100);

	//cornellBox();

	//Tone * rein = new ReinhardTone(200, 100);
	//rein->setLumType(Tone::DYNAMIC);
	//rein->setLumValue(5);
	//rein->setLumCoord(50, 5);
	//camera->setTone(rein);

	/*camera->setBGColor(0.04, 0.64, 0.8);
	camera->setRecurseDepth(5);
	camera->setIlluminationModel( new Phong() );*/
	//camera->setIlluminationModel(new BlinnPhong());

	//world.add( new LightSource(Point3(10, 0, -5), Colour(1, 1, 1)) );
	//world.add( new LightSource(Point3(0, 0, -20), Colour(1, .3, 1)) );

	//Colour texColors[] = {Colour(0, 1, 0), Colour(.1, .4, 0), Colour(.9, .5, 1)};
	//world.add(new Plane(Point3(0, -4, 0), Vector3(.1, 1, 0), 0, Texture::generateTiles(12, 12, 3, 3, texColors)));


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	\
	glShadeModel(GL_FLAT);

	glColor3f(1, 1, 1);
}


/*There's nothing exciting past this point; turn back!*/

void reshape(int w, int h)
{
	if (origWid == -1)
	{
		origWid = w;
		origHei = h;
	}
	wid = w;
	hei = h;
	aspect = wid / hei;

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 384);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	init();

	glutMainLoop();

	return 0;
}

