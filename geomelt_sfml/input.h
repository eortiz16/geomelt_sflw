#pragma once

#include "headers.h"
#include "generic.h"

/* Get coordinates via translation
* SFML:
*			Top left		(0      , 0)
*			Top Right		(SCR_WD , 0)
*			Bottom left		(0      , SCRN_HT)
*			Bottom Right	(SCRN_WD, SCRN_HT)
*
* GeoMelt (viewport):
*			Top left		(-1920,  1080)
*			Top Right		( 1920,  1080)
*			Bottom left		(-1920, -1080)
*			Bottom Right	( 1920, -1080)
*
* Translation on X Axis
*			ValX = ValX - (SCRN_WD - ValX);
*
* Translation on Y Axis
*			ValY = -ValY + (SCRN_HT - ValY);
*/

class Input {
public:
	// Method: SFML Mouse Coordinates -> GLViewport Coordinates
	static int translateX(int x);
	static int translateY(int y);
};