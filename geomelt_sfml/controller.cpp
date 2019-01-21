#include "controller.h"

ButtonMapping::ButtonMapping()
{
	MyButton button;
	
	button.myEventType = sf::Event::JoystickButtonPressed;
	button.myID1 = X;
	button.myID2 = Y;
	buttons[JUMP] = button;

	button.myEventType = sf::Event::JoystickButtonPressed;
	button.myID1 = A;
	button.myID2 = B;
	buttons[ATTACK] = button;

	button.myEventType = sf::Event::JoystickButtonPressed;
	button.myID1 = LB;
	button.myID2 = RB;
	buttons[SPECIAL] = button;
}

