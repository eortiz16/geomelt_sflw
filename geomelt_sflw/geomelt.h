#pragma once

#include "headers.h"
#include "menus.h"
#include "levels.h"
#include "camera.h"
#include "controller.h"

constexpr auto FPS = 60;
constexpr auto MS_PER_UPDATE() { return 1000.0f / (float) FPS; }

/*** Helper class to  synchronize the game loop and unbinds physics() and draw() from CPU speed ***/
class Sync {
private:
	sf::Clock game_clock;
	double previous;
	double current;
	double elapsed;
	double lag;

	friend class Game;
public:
	Sync();
};


class Game {
private:
	Sync sync;
	sf::ContextSettings contextSettings;
	unique_ptr<sf::RenderWindow> window;
	CurrentGameState current;
	Assets assets;
	unique_ptr<Level> level;
	unique_ptr<Menu> menu;
	Camera camera;

	friend class Input;
public:
	void process_input();
	void loop();
	Game();
	~Game();
};

