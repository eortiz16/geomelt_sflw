#pragma once

#include "headers.h"
#include "assets.h"
#include "complex_shapes.h"
#include "levels.h"
#include "controller.h"

constexpr auto JUMP_PARAM = 25.0f;
enum PlayerState { ALIVE, DEAD, ELIMINATED };
enum SelectColor{NEXT, PREV, RANDOM};

class Attributes {
private:
	float health;
	int lifeCount;
	PlayerState lifeState;

	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class Camera;
	friend class CharacterSelect;
public:
	Attributes();
};

class Toggle {
private:
	bool initDeath;
	bool attacking;
	bool walking;
	bool on_ground;
	sf::Clock deathTimer;
	sf::Clock attackTimer;

	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class Level;
	friend class Command;
	friend class CommandPlayer;
	friend class AttackCommand;
	friend class LeftCommand;
	friend class RightCommand;
public:
	Toggle();
};

class Player {
private:
	static unsigned int count; // num of players
	static vector<int> availableIDs;

	float weight;
	float move_param_x; // speed
	float move_param_y; // jump
	int JUMP_MAX;
	int jumpCount;
	Direction direction;

	unsigned int myID;
	Toggle toggle;
	Attributes stats;
	geomelt::Vec velocity;

	unique_ptr<geomelt::Shape> body;
	unique_ptr<geomelt::Shape> outline;
	unique_ptr<geomelt::Shape> reflection;
	geomelt::Quad arm;
	geomelt::Quad armOutline;
	geomelt::Circle eye;
	CharColorOptions myColor;

	//My friends
	friend class Level;
	friend class Field_Level;
	friend class Time_Level;
	friend class Night_Level;
	friend class CharacterSelect;
	friend class Boxy;
	friend class Ball;
	friend class Camera;
	friend class LevelState;
	friend class Game;
	friend class Command;
	friend class CommandPlayer;
	friend class AttackCommand;
	friend class LeftCommand;
	friend class RightCommand;
public:
	//ButtonMapping buttonMapping;
	void read_buttons(unsigned int button);
	void read_axis(unsigned int joyID);
	void read_keys(unsigned int key);
	void read_released_keys(unsigned int key);
	void simple_update();
	void simple_update_menu();
	void change_color(SelectColor option);
	void respawn();
	void death_handler();
	void update_reflection_x();
	unsigned int extract_lowest_ID();
	void attack();
	virtual void jump() = 0;
	virtual void special() = 0;
	void move(Direction dir);
	void reset_attributes();
	virtual void render(void) = 0;
	virtual void update_position(vector<Platform> plat) = 0;
	virtual void physics(vector<Platform> plat);
	
	Player& operator = (const Char_Color_Set &clr)
	{
		body->color = clr.body;
		outline->color = clr.outline;
		reflection->color = clr.reflection;
		arm.color = clr.body;
		return *this;
	}

	explicit Player();
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	virtual ~Player();
};

class Ball : public Player {
public:
	void render();
	void update_position(vector<Platform> plat);
	void jump();
	void exhale();
	void special() {}

	explicit Ball();
	Ball(const Ball&) = delete;
	Ball& operator=(const Ball&) = delete;
	~Ball() {}

	friend class LevelState;
};

class Boxy : public Player {
public:
	void render();
	void update_position(vector<Platform> plat);
	void jump();
	void special() {}
	
	explicit Boxy();
	Boxy(const Boxy&) = delete;
	Boxy& operator=(const Boxy&) = delete;
	~Boxy() {}

	friend class LevelState;
};
