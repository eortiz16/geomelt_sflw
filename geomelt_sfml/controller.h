#pragma once
#include "headers.h"
#include "assets.h"
#include "levels.h"

enum XboxAxis { LAX, LAY, LTRT, RAX, RAY, DPADX, DPADY };
enum XboxButton { A, B, X, Y, LB, RB, SELECT, START, L3, R3 };
enum PS4Button { SQUARE, CROSS, CIRCLE, TRIANGLE, LEFT1, RIGHT1, LEFT2, RIGHT2, SELECT_, START_, LEFT3, RIGHT3, HOME, TOUCH };
enum ActionType { JUMP, ATTACK, SPECIAL };

class MyButton {
private:
	sf::Event::EventType myEventType;
	sf::Joystick::Axis myAxes;
	unsigned int myID1;
	unsigned int myID2;

	friend class ButtonMapping;

};

class ButtonMapping {
private:
	map<ActionType, MyButton> buttons;
public:
	ButtonMapping();
};