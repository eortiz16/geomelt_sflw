#pragma once
#include "headers.h"

/*** Helper class to  synchronize the game loop and unbinds physics() and draw() from CPU speed ***/
class Sync {
private:
	sf::Clock game_clock;
	double previous;
	double current;
	double elapsed;
	double lag;

	friend class Game;
	friend class GFXNet;
public:
	Sync();
};