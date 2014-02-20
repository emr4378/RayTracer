#ifndef TONE_H
#define TONE_H

#include "colour.h"

#define	LMAX	100
#define	LDMAX	100

/**
 * Tone - abstract class
 *
 * Represents a image tone reproduction type. Contains methods that
 * allow single pixels or entire images to have the tone adjusted,
 * creating special imaging effects (HDR and bloom, for example).
 *
 * @author Eduardo Rodrigues - emr4378
 */
class Tone
{
public:
	struct LumCoord {
		int x;
		int y;
	};
	enum LumType{ DYNAMIC, COORD, VALUE };

	Tone() : lMax(LMAX), ldMax(LDMAX), lumType(LumType::DYNAMIC) {}
	Tone(float lM, float ldM) : lMax(lM), ldMax(ldM) {}
	virtual ~Tone() {}

	/**
	 * reproduce - abstract
	 *
	 * Reproduces an image tone on a color (specific pixel of an image), given the
	 * child class' implementation.
	 *
	 * @param avgLum - float, the average luminance of the image; only used if lumType = DYNAMIC
	 * @param col - Colour, the color to reproduce the tone on
	 */
	virtual void reproduce(float avgLum, Colour& col) = 0;

	/**
	 * reproduce
	 *
	 * Runs the single pixel/color tone reproduction method on every pixel
	 * in a given image. Depending on the luminance type (LumType) set,
	 * may use different "key" luminance values
	 *
	 * @param avgLum - float, the average luminance of the image; only used if lumType = DYNAMIC
	 * @param arr - Colour*, the image to reproduce the tone on
	 * @param rows, cols - int, the dimensions of the (Colour*)arr
	 */
	void reproduce(float avgLum, Colour * arr, int rows, int cols) {
		Colour pixCol;

		float lumVal = avgLum;
		if (lumType == LumType::VALUE) {
			lumVal = lumValue;
		} else if (lumType== LumType::COORD) {
			lumCoord.x > cols ? lumCoord.x = cols : lumCoord.x < 0 ? lumCoord.x = 0 : 0;
			lumCoord.y > rows ? lumCoord.y = rows : lumCoord.y < 0 ? lumCoord.y = 0 : 0;
			lumVal = arr[lumCoord.y * cols + lumCoord.x].getLuminance();
		}

		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				pixCol = Colour(arr[y * cols + x].r,
								arr[y * cols + x].g,
								arr[y * cols + x].b);

				pixCol *= lMax;;

				reproduce(lumVal, pixCol);
				pixCol *= 1.0f/ldMax;

				arr[y * cols + x].r = pixCol.r;
				arr[y * cols + x].g = pixCol.g;
				arr[y * cols + x].b = pixCol.b;
			}
		}
	}

	/*BEGIN: Getters & Setters*/
	void setLumType(LumType t) {
		lumType = t;
	}
	LumType getLumType() {
		return lumType;
	}

	void setLumCoord(int x, int y) {
		lumCoord.x = x;
		lumCoord.y = y;
		setLumType(LumType::COORD);
	}
	LumCoord getLumCoord() {
		return lumCoord;
	}

	void setLumValue(float v) {
		lumValue = v;
		setLumType(LumType::VALUE);
	}
	float getLumValue() {
		return lumValue;
	}

	float getLMax() {
		return lMax;
	}
	void setLMax(float l) {
		lMax = l;
	}

	float getLDMax() {
		return ldMax;
	}
	void setLDMax(float l) {
		ldMax = l;
	}
	/*END: Getters & Setters*/

protected:
	LumType lumType;	//the type of luminance value to use
	LumCoord lumCoord;	//coordinate in the image whose luminance value will be used if lumType == COORD
	float lumValue;		//the luminance value to be used if lumType == VALUE
	float lMax;		//maximum luminance from scene
	float ldMax;	//maximum luminance of display
};

#endif