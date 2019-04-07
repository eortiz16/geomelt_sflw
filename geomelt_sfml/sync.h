#pragma once
#include "headers.h"

constexpr auto FPS = 60;
constexpr auto MS_PER_UPDATE() { return 1000.0f / (float)FPS; }

/*** Helper class to  synchronize the game loop and unbinds physics() and draw() from CPU speed ***/
class Sync {
private:
	static sf::Clock gameClock;
	static double previous;
	static double current;
	static double elapsed;
	static double lag;
public:
	static void update();
	static void catch_up();
	Sync();

	friend class GFXNet;
};