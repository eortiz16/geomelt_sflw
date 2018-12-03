#pragma once

#include "headers.h"
#include "menus.h"
#include "levels.h"
#include "camera.h"
#include "controller.h"

constexpr auto FPS = 60;
constexpr auto MS_PER_UPDATE() { return 1000.0f / (float) FPS; }

class Sync {
private:
	friend class Game;
public:
	sf::Clock game_clock;
	double previous;
	double current;
	double elapsed;
	double lag;
	Sync();
};

class Input {
private:
	friend class Game;
public:
	void process(Game *game);
};

class Game {
private:

public:	
	Sync sync;
	ContextSettings contextSettings;
	unique_ptr<RenderWindow> window;
	CurrentGameState current;
	Assets assets;
	unique_ptr<Level> level;
	unique_ptr<Menu> menu;
	Camera camera;
	Input input;

	void loop();

	unique_ptr<RenderWindow> &get_window() { return window; }

	Game();
	~Game();
};

