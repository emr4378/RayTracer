#include "camera.h"

using namespace std;

Camera::Camera() :	position(),
	forward(0, 0, -1),
	up(0, 1, 0),
	model(0),
	tone(0),
	world(0),
	recurseDepth(1),
	shadows(false),
	bgColor(DF_BG_R, DF_BG_G, DF_BG_B) {}

Camera::Camera(IlluminationModel* m) : position(),
	forward(0, 0, -1),
	up(0, 1, 0),
	model(m),
	tone(0),
	world(0),
	recurseDepth(3),
	shadows(true),
	bgColor(DF_BG_R, DF_BG_G, DF_BG_B) {}
Camera::~Camera()
{
	delete model;
	delete tone;
	delete world;
}

void Camera::lookAt(Point3 p, Point3 c, Vector3 u)
{
	setPosition(p);
	setForward(c - p);
	setUp(u);
}
void Camera::lookAt(double x, double y, double z,
                    double cx, double cy, double cz,
                    double ux, double uy, double uz)
{
	setPosition(x, y, z);
	setForward(cx - x, cy - y, cz - z);
	setUp(ux, uy, uz);
}

void Camera::setOrientation(Point3 p, Vector3 f, Vector3 u)
{
	setPosition(p);
	setForward(f);
	setUp(u);
}

void Camera::setOrientation(double x, double y, double z,
                            double fx, double fy, double fz,
                            double ux, double uy, double uz)
{
	setPosition(x, y, z);
	setForward(fx, fy, fz);
	setUp(ux, uy, uz);
}

void Camera::setDimensions(double wid, double hei, double zFar, double pSize)
{
	setWidth(wid);
	setHeight(hei);
	setDistance(zFar);
	setPixelSize(pSize);
}

void Camera::setPerspective(double fov, double aspect, double zNear, double pSize)
{
	setHeight(tan(fov * PI / 360) * zNear * 2);
	setWidth(aspect * getHeight());
	setDistance(zNear);
	setPixelSize(pSize);
}

Colour* Camera::render(double sWid, double sHei)
{
	if (world == 0)
	{
		return 0;
	}

	if (model == 0)
	{
		model = new Flat();
	}


	World& w = *world;
	int rows = (int)ceil(sHei / pixelSize);
	int cols = (int)ceil(sWid / pixelSize);
	Colour* arr = new Colour[rows * cols];
	for (int i = 0; i < rows * cols; i++)
	{
		arr[i] = Colour();
	}

	double logAvg = 0;

	double xInc = width / cols;
	double yInc = height / rows;

	Vector3 right = forward ^ up;
	right.normalize();

	Point3 curPos(position);
	curPos += forward * distance;
	curPos -= up * (height / 2);
	curPos -= right * (width / 2);

	up *= yInc;
	right *= xInc;

	Colour pixCol;

	double smallOff = 0.01;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			Vector3 dir = curPos - position;
			Ray ray(position, dir);
			pixCol += illuminate(0, ray, recurseDepth);

			arr[y * cols + x].r = pixCol.r;
			arr[y * cols + x].g = pixCol.g;
			arr[y * cols + x].b = pixCol.b;

			//calculate luminance of pixel
			if (tone != 0)
			{
				pixCol *= tone->getLMax();
				logAvg += log(smallOff + pixCol.getLuminance());
			}

			pixCol *= 0;
			curPos += right;
		}
		curPos -= right * cols;
		curPos += up;
	}

	//apply tone reproduction, if it exists
	if (tone != 0)
	{
		logAvg = exp(logAvg / (rows * cols));
		tone->reproduce(logAvg, arr, rows, cols);
	}

	up.normalize();
	forward.normalize();

	return arr;
}

Colour Camera::illuminate(Object3D* prevHit, const Ray& oRay, int depth)
{
	World& w = *world;
	int samples = 1;
	if (superSample)
	{
		samples = 4;
	}
	Vector3 offsets[5] = {	Vector3(),
	                        Vector3(-SS_OFF, 0, 0),
	                        Vector3(0, -SS_OFF, 0),
	                        Vector3(SS_OFF, 0, 0),
	                        Vector3(0, SS_OFF, 0)
	                     };

	vector<LightSource*>::iterator iter;
	vector<Object3D*>::iterator oIter;

	Colour pixCol;

	for (int s = 0; s < samples; s++)
	{
		Ray ray(oRay);
		ray.direction += offsets[s];
		ray.direction.normalize();

		Object3D* closest = 0;
		double closestTime = FLT_MAX;
		double tempTime;

		for (oIter = w.objectList.begin(); oIter != w.objectList.end(); oIter++)
		{
			if (*oIter != prevHit)
			{
				tempTime = (*oIter)->intersect(ray);
				if (tempTime >= 0 && tempTime < closestTime)
				{
					closest = (*oIter);
					closestTime = tempTime;
				}
			}
		}
		if (closest != 0)
		{
			IntersectData oIntData = closest->evaluateIntersect(ray, closestTime);
			IntersectData intData;
			vector<IntersectData> intersects;
			bool inShadow;

			for (iter = w.lightList.begin(); iter != w.lightList.end(); iter++)
			{
				intData = oIntData;
				inShadow = false;
				intData.incoming = (*iter)->position - intData.point;

				if (shadows)
				{
					double hitTime;
					Object3D* blocker;
					for (oIter = w.objectList.begin(); !inShadow && oIter != w.objectList.end(); oIter++)
					{
						if (*oIter != closest)
						{
							hitTime = (*oIter)->intersect(Ray((*iter)->position, -intData.incoming));
							if (hitTime >= 0 && hitTime < closestTime)
							{
								inShadow = true;
								blocker = *oIter;
							}
						}
					}
				}

				if (!inShadow)
				{
					intData.incoming.normalize();
					intData.reflection = Vector3::reflect(intData.incoming, intData.normal);
					intData.reflection.normalize();
					intData.light = (*iter);
					intersects.push_back(intData);
				}
			}

			if (intersects.empty())
			{
				intersects.push_back(oIntData);
			}

			pixCol += model->illuminate(intersects);
			if (depth > 0)
			{
				double refl = oIntData.material.getReflectiveCoeff();
				double transm = oIntData.material.getTransmissiveCoeff();
				Vector3 reflectionDir = Vector3::reflect(-oIntData.direction, oIntData.normal);
				reflectionDir.normalize();
				Ray reflectRay(oIntData.point, reflectionDir);

				if (refl > 0)
				{
					Colour tempRefl = illuminate(closest, reflectRay, depth - 1);
					tempRefl *= refl;
					pixCol += tempRefl;
				}

				if (transm > 0)
				{
					double n = oIntData.material.getRefractionIndex();
					Vector3 D = -oIntData.direction;
					Vector3 N = oIntData.normal;

					double nDot = -D * N;
					if (nDot > 0)
					{
						N *= -1;
					}

					double disc = 1 + (n * n * (nDot * nDot - 1));
					if (disc < 0)
					{
						Colour tempTrans = illuminate(closest, reflectRay, depth - 1);
						tempTrans *= (refl + transm);
						pixCol += tempTrans;
					}
					else
					{
						double thing = n * (nDot) - sqrtl(disc);
						Vector3 transDir = n * D + (thing * N);

						Ray transRay(oIntData.point, transDir);

						Colour tempTrans = illuminate(closest, transRay, depth - 1);
						tempTrans *= transm;
						pixCol += tempTrans;
					}
				}
			}
		}
		else
		{
			pixCol += bgColor;
		}
	}

	pixCol *= (1.0 / samples);

	return pixCol;
}

/*BEGIN: Getters & Setters*/
Point3& Camera::getPosition()
{
	return position;
}
const Point3& Camera::getPosition() const
{
	return position;
}
void Camera::setPosition(Point3 p)
{
	position = p;
}
void Camera::setPosition(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

Vector3& Camera::getForward()
{
	return forward;
}
const Vector3& Camera::getForward() const
{
	return forward;
}
void Camera::setForward(Vector3 f)
{
	forward = f;
	forward.normalize();
}
void Camera::setForward(double x, double y, double z)
{
	forward.x = x;
	forward.y = y;
	forward.z = z;
	forward.normalize();
}

Vector3& Camera::getUp()
{
	return up;
}
const Vector3& Camera::getUp() const
{
	return up;
}
void Camera::setUp(Vector3 u)
{
	up = u;
	up.normalize();
}
void Camera::setUp(double x, double y, double z)
{
	up.x = x;
	up.y = y;
	up.z = z;
	up.normalize();
}

int Camera::getRecurseDepth()
{
	return recurseDepth;
}
void Camera::setRecurseDepth(int d)
{
	recurseDepth = d;
}

double Camera::getWidth()
{
	return width;
}
void Camera::setWidth(double w)
{
	width = w;
}

double Camera::getHeight()
{
	return height;
}
void Camera::setHeight(double h)
{
	height = h;
}

double Camera::getDistance()
{
	return distance;
}
void Camera::setDistance(double d)
{
	distance = d;
}

double Camera::getPixelSize()
{
	return pixelSize;
}
void Camera::setPixelSize(double p)
{
	pixelSize = p;
}

bool Camera::isShadowsEnabled()
{
	return shadows;
}
void Camera::setShadowsEnabled(bool s)
{
	shadows = s;
}

void Camera::enableShadows()
{
	shadows = true;
}

void Camera::disableShadows()
{
	shadows = false;
}

bool Camera::isSuperSampling()
{
	return superSample;
}
void Camera::setSuperSampling(bool s)
{
	superSample = s;
}

Colour& Camera::getBGColor()
{
	return bgColor;
}

const Colour& Camera::getBGColor() const
{
	return bgColor;
}

void Camera::setBGColor(Colour c)
{
	bgColor = c;
}

void Camera::setBGColor(double r, double g, double b)
{
	bgColor.r = r;
	bgColor.g = g;
	bgColor.b = b;
}

IlluminationModel* Camera::getIlluminationModel()
{
	return model;
}
void Camera::setIlluminationModel(IlluminationModel& m)
{
	delete model;
	model = &m;
}
void Camera::setIlluminationModel(IlluminationModel* m)
{
	delete model;
	model = m;
}

Tone* Camera::getTone()
{
	return tone;
}
void Camera::setTone(Tone& t)
{
	delete tone;
	tone = &t;
}
void Camera::setTone(Tone* t)
{
	delete tone;
	tone = t;
}

World* Camera::getWorld()
{
	return world;
}
void Camera::setWorld(World& w)
{
	delete world;
	world = &w;
}
void Camera::setWorld(World* w)
{
	delete world;
	world = w;
}


/*END: Getters & Setters*/