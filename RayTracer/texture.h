#ifndef TEXTURE_H
#define TEXTURE_H

#include "colour.h"

//TODO: Figure out texture mapping/texture coordinates

/**
 * Texture
 *
 * Represents a texture object, which can be used instead of a Material on objects.
 * Set up so that loading a real texture into it is a possibility.
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Texture
{
public:
	/**
	 * generateTiles - static method
	 *
	 * Generates a grid/tile pattern of the given colors
	 *
	 * @param width, height - int, the dimensions of the texture
	 * @param tileSize - int, the size of each individual tile
	 * @param numTiles - int, the number of times to tile (possibly redundant!)
	 * @return Texture* - a pointer to the tile texture generated
	 */
	static Texture* generateTiles(int width, int height, int tileSize, int numTiles, Colour colours[]);

	Texture(int width, int height);
	Texture(int width, int height, char* imgPath);
	~Texture();

	/*BEGIN: Getters & Setters*/
	int getWidth();
	int getHeight();

	void getColour(double u, double v, Colour& col) const;
	void setColour(double u, double v, Colour& col);

	bool isWrapping();
	void setWrapping(bool w);
	/*END: Getters & Setters*/

private:
	int _width;
	int _height;
	unsigned char* _data;	//the texture map, in array form
	bool wrap;
};

#endif