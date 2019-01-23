#pragma once

#include "headers.h"
#include "menus.h"
#include "levels.h"
#include "camera.h"
#include "sync.h"
#include "input.h"
#include "command.h"

constexpr auto FPS = 60;
constexpr auto MS_PER_UPDATE() { return 1000.0f / (float)FPS; }

class RState;
class Command;

class GFXNet {
private:
	RState* _state;
	unique_ptr<Level> level;
	sf::ContextSettings contextSettings;
	Sync sync;
	unique_ptr<sf::RenderWindow> window;
	unique_ptr<Command> command;
public:
	void setState(RState* state);
	void next();
	void prev();
	void handler();
	void loop();

	GFXNet();
	~GFXNet() {}

	friend class Game;
	friend class RState;
	friend class MainMenuState;
	friend class CharacterSelectState;
	friend class LvlSelectState;
	friend class LevelState;
	friend class PauseState;
};

class RState {
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual void handler() = 0;
	virtual void read_input() = 0;

	RState() {}
	RState(GFXNet* context) {}
	virtual ~RState() {}
	friend class Game;
};

class MainMenuState : public RState {
private:
	GFXNet* _context;
	MainMenu menu;
public:
	void next();
	void prev();
	void handler();
	void read_input();

	MainMenuState() {}
	MainMenuState(GFXNet* context);
	~MainMenuState() {}
};

class CharacterSelectState : public RState {
private:
	GFXNet* _context;
	CharacterSelect menu;
public:
	void next();
	void prev();
	void handler();
	void read_input();

	CharacterSelectState() {}
	CharacterSelectState(GFXNet* context);
	~CharacterSelectState() {}
};

class LvlSelectState : public RState {
private:
	GFXNet* _context;
	LevelSelect menu;
public:
	void next();
	void prev();
	void handler();
	void read_input();

	LvlSelectState() {}
	LvlSelectState(GFXNet* context);
	~LvlSelectState() {}
};

class LevelState : public RState {
private:
	GFXNet* _context;
public:
	void next();
	void prev();
	void handler();
	void read_input();

	LevelState() {}
	LevelState(GFXNet* context);
	~LevelState() {}
};

class PauseState : public RState {
private:
	GFXNet* _context;
public:
	void next();
	void prev();
	void handler();
	void read_input();

	PauseState() {}
	PauseState(GFXNet* context);
	~PauseState() {}
};