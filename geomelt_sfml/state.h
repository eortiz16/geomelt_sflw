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
class Menu;
class Level;

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

	friend class Input;

	friend class RState;
	friend class MainMenuState;
	friend class CharacterSelectState;
	friend class LevelSelectState;
	friend class LevelState;
	friend class PauseState;
	
	friend class Command;
	friend class CommandMainMenu;
	friend class PrevCommand;
	friend class NextCommand;
	friend class ConfirmCommand;
	friend class DenyCommand;
	friend class ExitCommand;

	friend class JumpCommand;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
	friend class StopCommand;
	friend class CommandLevel;
	friend class RemoveCharacterCommand;

	friend class ChangeCharacterCommand;
	friend class AddCharacterCommand;
	friend class PrevColorCommand;
	friend class NextColorCommand;
};

class RState {
private:
	GFXNet* _context;
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual void handler() = 0;
	virtual void read_input() = 0;

	RState() {}
	RState(GFXNet* context) {}
	virtual ~RState() {}
	
	friend class Input;

	friend class MainMenuState;
	friend class CharacterSelectState;
	friend class LevelSelectState;
	friend class LevelState;
	friend class PauseState;
	friend class JumpCommand;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
	friend class StopCommand;
	friend class CommandLevel;
	friend class ConfirmCommand;
	friend class DenyCommand;
	friend class ExitCommand;

	friend class ChangeCharacterCommand;
	friend class AddCharacterCommand;
	friend class RemoveCharacterCommand;
	friend class PrevColorCommand;
	friend class NextColorCommand;
};

class MenuState : public RState {
private:
	unique_ptr<Menu> menu;
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual void handler() = 0;
	virtual void read_input() = 0;

	MenuState() {}
	MenuState(GFXNet* context) {}
	~MenuState() {}

	friend class MainMenuState;
	friend class CharacterSelectState;
	friend class LevelSelectState;
	friend class PauseState;
	friend class CommandMainMenu;
	friend class PrevCommand;
	friend class NextCommand;
	friend class ConfirmCommand;
	friend class DenyCommand;

	friend class AddCharacterCommand;
	friend class RemoveCharacterCommand;
	friend class PrevColorCommand;
	friend class NextColorCommand;
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
	friend class PrevCommand;
	friend class NextCommand;
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

class LevelSelectState : public MenuState {
public:
	void next();
	void prev();
	void handler();
	void read_input();

	LevelSelectState() {}
	LevelSelectState(GFXNet* context);
	~LevelSelectState() {}
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