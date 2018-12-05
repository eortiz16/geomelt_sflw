#pragma once

#include "headers.h"
#include "menus.h"
#include "levels.h"
#include "camera.h"
#include "controller.h"

constexpr auto FPS = 60;
constexpr auto MS_PER_UPDATE() { return 1000.0f / (float) FPS; }

/*** Sync is a class to help synchronize the game loop ***/
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
	ContextSettings contextSettings;
	unique_ptr<RenderWindow> window;
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

