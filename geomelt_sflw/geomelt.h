#pragma once

#include "headers.h"
#include "menus.h"
#include "levels.h"
#include "camera.h"
#include "controller.h"

#define FPS 60
#define MS_PER_UPDATE 1000.0f / (float) FPS

class Sync {
public:
	sf::Clock game_clock;
	double previous;
	double current;
	double elapsed;
	double lag;
	Sync();
};

class Input {
public:
	void process(Game *game);
};

class Game {
public:	
	Sync sync;
	ContextSettings contextSettings;
	unique_ptr<RenderWindow> window;

	//ENUMERATIONS
	Render_State render;
	CurrentMenu currentMenu;
	bool menuChange;

	Assets assets;
	
	unique_ptr<Level> level;
	unique_ptr<Menu> menu;

	Camera camera;
	Input input;

	Game();
	~Game();
};

