#ifndef CAMERA_H
#define	CAMERA_H

#include <windows.h>
#include <process.h>
#include <iostream>

#include "vecmath.h"
#include "world.h"
#include "illuminationmodel.h"
#include "flat.h"
#include "tone.h"

#define SS_OFF	0.005
#define DF_BG_R	0
#define DF_BG_G	0
#define DF_BG_B	0

/**
 * Camera
 *
 * A representation of a camera in 3d space. Stores all the properties used
 * in determining what the output image will look like, such as pixel size,
 * illumination model, tone reproducer, recurse depth, and background color
 *
 * Modelled after the OpenGL/GLUT camera, contains convenience methods like
 * setOrientation, lookAt, and setPerspective
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Camera
{
public:

	Camera();
	Camera(IlluminationModel* m);
	~Camera();

	/*
	 * lookAt
	 *
	 * Similar to gluLookAt, orients the camera to focus on a point
	 *
	 * @param p - Point3, the center position of the camera
	 * @param c - Point3, the point to look at
	 * @param u - Vector3, the up vector
	 */
	void lookAt(Point3 p, Point3 c, Vector3 u);

	/*
	 * lookAt
	 *
	 * Similar to gluLookAt, orients the camera to focus on a point
	 *
	 * @param x,y,z - double, the coordinates of the camera
	 * @param cx, cy, cz - double, the coordinates of point to look at
	 * @param ux, uy, uz - double, the componenents of the up vector
	 */
	void lookAt(double x, double y, double z,
	            double cx, double cy, double cz,
	            double ux, double uy, double uz);

	/**
	 * setOrientation
	 *
	 * Orients the camera, given it's center point and up and forward vectors
	 *
	 * @param p - Point3, the center position of the cmaera
	 * @param f - Vector3, the forward axis
	 * @param u - Vector3, the up vector
	 */
	void setOrientation(Point3 p, Vector3 f, Vector3 u);

	/**
	 * setOrientation
	 *
	 * Orients the camera, given it's center point and up and forward vectors
	 *
	 * @param x,y,z - double, the coordinates of the camera
	 * @param fx, fy, fz - double, the components of the forward vector
	 * @param ux, uy, uz - double, the componenents of the up vector
	 */
	void setOrientation(double x, double y, double z,
	                    double fx, double fy, double fz,
	                    double ux, double uy, double uz);

	/**
	 * setOrientation
	 *
	 * Similar to gluFrustum, sets up the viewing plane position and dimensions
	 *
	 * @param wid, hei - double, the width and height of the image plane
	 * @param zFar - double, how far away the image plane is
	 * @param pSize - double, the pixel size to be used.
	 */
	void setDimensions(double wid, double hei, double zFar, double pSize);

	/**
	 * setPerspective
	 *
	 * Similar to gluPerspective, sets up the view plane position and dimensions
	 *
	 * @param fovy - double, the field of view of the camera (left-right spread angle)
	 * @param aspect - double, the aspect ratio of the screen
	 * @param zNear - double, how far away the image plane is
	 * @param pSize - double, the pixel size to be used
	 */
	void setPerspective(double fovy, double aspect, double zNear, double pSize);

	/**
	 * render
	 *
	 * Where the magic happens; the actual ray casting occurs here, and the resulting
	 * colour for each pixel is stored in an array that can then be iterated
	 * through to place the image onto a screen
	 *
	 * @param sWid, sHei - the dimensions of the scene to be rendered
	 */
	Colour* render(double sWid, double sHei);


	/*BEGIN: Getters & Setters*/
	Point3& getPosition();
	const Point3& getPosition() const;
	void setPosition(Point3 p);
	void setPosition(double x, double y, double z);

	Vector3& getForward();
	const Vector3& getForward() const;
	void setForward(Vector3 f);
	void setForward(double x, double y, double z);

	Vector3& getUp();
	const Vector3& getUp() const;
	void setUp(Vector3 u);
	void setUp(double x, double y, double z);

	int getRecurseDepth();
	void setRecurseDepth(int d);

	double getWidth();
	void setWidth(double w);

	double getHeight();
	void setHeight(double h);

	double getDistance();
	void setDistance(double d);

	double getPixelSize();
	void setPixelSize(double p);

	bool isShadowsEnabled();
	void setShadowsEnabled(bool s);
	void enableShadows();
	void disableShadows();

	bool isSuperSampling();
	void setSuperSampling(bool s);

	Colour& getBGColor();
	const Colour& getBGColor() const;
	void setBGColor(Colour c);
	void setBGColor(double r, double g, double b);

	IlluminationModel* getIlluminationModel();
	void setIlluminationModel(IlluminationModel& m);
	void setIlluminationModel(IlluminationModel* m);

	Tone* getTone();
	void setTone(Tone& t);
	void setTone(Tone* t);

	World* getWorld();
	void setWorld(World& w);
	void setWorld(World* w);
	/*END: Getters & Setters*/




private:
	Colour illuminate(Object3D* prevHit, const Ray& oRay, int depth);

	Point3 position;
	Vector3 forward;
	Vector3 up;
	int recurseDepth;			//the max number of reflections/transmissions that can occur
	double width;
	double height;
	double distance;
	double pixelSize;
	bool shadows;				//if true, shadows are rendered
	bool superSample;			//if true, super sampling is used (4 samples)
	Colour bgColor;				//the color that is used if the cast ray intersects nothing
	IlluminationModel* model;	//Illumination model to be used on all objects in the scene
	Tone* tone;				//Tone Reproduction to be used in final image
	World* world;
};

#endif