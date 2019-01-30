#pragma once
#include "headers.h"

constexpr auto FPS = 60;
constexpr auto MS_PER_UPDATE() { return 1000.0f / (float)FPS; }

/*** Helper class to  synchronize the game loop and unbinds physics() and draw() from CPU speed ***/
class Sync {
private:
	sf::Clock game_clock;
	double previous;
	double current;
	double elapsed;
	double lag;
public:
	void update();
	void catch_up();
	Sync();

	friend class GFXNet;
};