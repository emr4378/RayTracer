#include "texture.h"

#include <glut.h>
#include "Libraries\SOIL.h"

using namespace std;

Texture * Texture::generateTiles(int width, int height, int tileSize, int numTiles, Colour colours[]) {
	Texture * texture = new Texture(width, height);

	int startPos = 0;
	int pos = 0;
	for (int v = 0; v < height; v ++) {
		pos = startPos;
		for (int u = 0; u < width; u ++) {
			if (u % tileSize == 0) {
				pos = (pos + 1) % numTiles;
			}
			texture->setColour(u, v, colours[pos]);
		}
		if (v % tileSize == 0) {
			startPos = (startPos + 1) % numTiles;
		}
	}

	return texture;
}

Texture::Texture(int width, int height) : _width(width), _height(height)
{
	_data = new unsigned char[width * height * 3];
}

Texture::Texture(int width, int height, char * imgPath) : _width(width), _height(height) 
{
	_data = new unsigned char[_width * _height * 3];

	//TODO: Really? Bringing in SOIL just for this? You're better than that!

	GLuint ret = SOIL_load_OGL_texture(
		imgPath,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_COMPRESS_TO_DXT
		| SOIL_FLAG_DDS_LOAD_DIRECT
		| SOIL_FLAG_INVERT_Y
    );

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, ret);
	glGetTexImage(GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE, _data);
}


Texture::~Texture()
{
	delete [] _data;
}

int Texture::getWidth() {
	return _width;
}

int Texture::getHeight() {
	return _height;
}

Colour& Texture::getColour(float u, float v) const {
	if (!wrap) {
		u < 0 ? u = 0 : u > _width-1 ? u = _width - 1 : 0;
		v < 0 ? v = 0 : v > _height-1 ? v = _height - 1 : 0;
	} else {
		while (u < 0) {
			u += _width;
		}
		while (v < 0) {
			v += _height;
		}
		while (u > _width-1) {
			u -= _width;
		}
		while (v > _height-1) {
			v -= _height;
		}
	}
	int pos = (v * _width * 3) + (u * 3);
	return Colour(_data[pos+0]/255.0f, _data[pos+1]/255.0f, _data[pos+2]/255.0f);
}

void Texture::setColour(float u, float v, Colour& col) {
	int pos = (v * _width * 3) + (u * 3);
	_data[pos+0] = col.r * 255;
	_data[pos+1] = col.g * 255;
	_data[pos+2] = col.b * 255;
}

bool Texture::isWrapping() {
	return wrap;
}

void Texture::setWrapping(bool w) {
	wrap = w;
}