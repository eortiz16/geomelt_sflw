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
	
	friend class Command;
	friend class CommandMainMenu;
	friend class NavLeftCommand;
	friend class NavRightCommand;
	friend class NavUpCommand;
	friend class NavDownCommand;
	friend class ConfirmCommand;
	friend class DenyCommand;

	friend class JumpCommand;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
};

class RState {
protected:
	GFXNet* _context;
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual void handler() = 0;
	virtual void read_input() = 0;

	RState() {}
	RState(GFXNet* context) {}
	virtual ~RState() {}
	friend class Game;
	friend class JumpCommand;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
};

class MenuState : public RState {
protected:
	unique_ptr<Menu> menu;
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual void handler() = 0;
	virtual void read_input() = 0;

	MenuState() {}
	MenuState(GFXNet* context);
	~MenuState() {}

	friend class CommandMainMenu;
	friend class NavLeftCommand;
	friend class NavRightCommand;
	friend class NavUpCommand;
	friend class NavDownCommand;
	friend class ConfirmCommand;
	friend class DenyCommand;
};

class MainMenuState : public MenuState {
public:
	void next();
	void prev();
	void handler();
	void read_input();

	MainMenuState() {}
	MainMenuState(GFXNet* context);
	~MainMenuState() {}

	friend class CommandMainMenu;
	friend class NavLeftCommand;
	friend class NavRightCommand;
	friend class NavUpCommand;
	friend class NavDownCommand;
	friend class ConfirmCommand;
	friend class DenyCommand;
};

class CharacterSelectState : public MenuState {;
public:
	void next();
	void prev();
	void handler();
	void read_input();

	CharacterSelectState() {}
	CharacterSelectState(GFXNet* context);
	~CharacterSelectState() {}
};

class LvlSelectState : public MenuState {
public:
	void next();
	void prev();
	void handler();
	void read_input();

	LvlSelectState() {}
	LvlSelectState(GFXNet* context);
	~LvlSelectState() {}
};

class PauseState : public MenuState {
public:
	void next();
	void prev();
	void handler();
	void read_input();

	PauseState() {}
	PauseState(GFXNet* context);
	~PauseState() {}
};


class LevelState : public RState {
public:
	void next();
	void prev();
	void handler();
	void read_input();

	LevelState() {}
	LevelState(GFXNet* context);
	~LevelState() {}
};