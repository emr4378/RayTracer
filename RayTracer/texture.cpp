#include "texture.h"

#include <glut.h>

using namespace std;

Texture* Texture::generateTiles(int width, int height, int tileSize, int numTiles, Colour colours[])
{
	Texture* texture = new Texture(width, height);

	int startPos = 0;
	int pos = 0;
	for (int v = 0; v < height; v ++)
	{
		pos = startPos;
		for (int u = 0; u < width; u ++)
		{
			if (u % tileSize == 0)
			{
				pos = (pos + 1) % numTiles;
			}
			texture->setColour(u, v, colours[pos]);
		}
		if (v % tileSize == 0)
		{
			startPos = (startPos + 1) % numTiles;
		}
	}

	return texture;
}

Texture::Texture(int width, int height) : _width(width), _height(height)
{
	_data = new unsigned char[width * height * 3];
}

Texture::~Texture()
{
	delete [] _data;
}

int Texture::getWidth()
{
	return _width;
}

int Texture::getHeight()
{
	return _height;
}

void Texture::getColour(double u, double v, Colour& col) const
{
	if (!wrap)
	{
		u < 0 ? u = 0 : u > _width - 1 ? u = _width - 1 : 0;
		v < 0 ? v = 0 : v > _height - 1 ? v = _height - 1 : 0;
	}
	else
	{
		while (u < 0)
		{
			u += _width;
		}
		while (v < 0)
		{
			v += _height;
		}
		while (u > _width - 1)
		{
			u -= _width;
		}
		while (v > _height - 1)
		{
			v -= _height;
		}
	}
	int pos = (v * _width * 3) + (u * 3);

	col.r = _data[pos + 0] / 255.0;
	col.g = _data[pos + 1] / 255.0;
	col.b = _data[pos + 2] / 255.0;
}

void Texture::setColour(double u, double v, Colour& col)
{
	int pos = (v * _width * 3) + (u * 3);
	_data[pos + 0] = col.r * 255;
	_data[pos + 1] = col.g * 255;
	_data[pos + 2] = col.b * 255;
}

bool Texture::isWrapping()
{
	return wrap;
}

void Texture::setWrapping(bool w)
{
	wrap = w;
}