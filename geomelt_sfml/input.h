#pragma once

#include "headers.h"
#include "generic.h"
#include "command.h"

//sf::Joystick::getIdentification(event.joystickButton.joystickId).vendorId
// Microsoft = 1118
// Sony = 1356
// logitech Controller = 1133

constexpr unsigned int KEY_ID = -1;

namespace xbox {
	enum Axis { LAX, LAY, LTRT, RAX, RAY, DPADX, DPADY };
	enum Button { A, B, X, Y, LB, RB, SELECT, START, L3, R3 };
}

namespace ps {
	enum Button { SQUARE, CROSS, CIRCLE, TRIANGLE, L1, R1, L2, R2, SELECT, START, L3, R3, HOME, TOUCH };
}

enum ActionType { JUMP, ATTACK, SPECIAL };

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
	static int translateX(int x);
	static int translateY(int y);
};
