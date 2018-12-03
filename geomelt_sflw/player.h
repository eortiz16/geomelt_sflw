#pragma once

#include "headers.h"
#include "assets.h"
#include "complex_shapes.h"
#include "levels.h"
#include "controller.h"

constexpr auto MOVE_PARAM_X = 15.0f;
constexpr auto MOVE_PARAM_Y = 5.0f;
constexpr auto JUMP_PARAM = 25.0f;

class Level;
class Field_Level;
class Night_Level;
class Time_Level;
class Camera;

enum PlayerState { ALIVE, DEAD, ELIMINATED };
enum SelectColor{NEXT, PREV, RANDOM};
class buttonMapping;
class Camera;

class Attributes {
private:
	float health;
	int lifeCount;
	PlayerState lifeState;

	//My Friends
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
	bool walk_toggle;
	bool on_ground;
	sf::Clock deathTimer;
	sf::Clock attackTimer;

	//My friends
	friend class Player;
public:
	Toggle();
	void xor_attack();
	void set_attack_toggle(bool);
	void set_walk_toggle(bool);
	void set_ground(bool);
	void set_initdeath(bool);
	void reset_attack_timer();
	void reset_death_timer();
	float get_death_time();
	int get_attack_time();
	bool get_initdeath();
	bool is_attacking();
	bool is_walking();
	bool is_on_ground();
};

class Player {
private:
	static unsigned int count;
	static vector<int> availableIDs;
	unsigned int myID;
	Toggle toggle;
	Attributes stats;

	//My friends
	friend class Level;
	friend class Field_Level;
	friend class Time_Level;
	friend class Night_Level;
	friend class CharacterSelect;
	friend class Boxy;
	friend class Ball;
	friend class Camera;
	friend class Game;
public:
	unsigned int get_count();
	Vec	velocity;
	bool created;

	//ButtonMapping buttonMapping;
	void read_buttons(unsigned int button);
	void read_axes(unsigned int joyID);
	
	void simple_update();
	void simple_update_menu();

	CharColorOptions myColor;

	medmelt::Shape *body;
	medmelt::Shape *outline;
	medmelt::Shape *reflection;
	medmelt::Quad arm;
	medmelt::Quad armOutline;
	medmelt::Circle eye;

	void change_color(SelectColor option);

	int JUMP_MAX;
	int jumpCount;
	Direction direction;

	void respawn();
	void death_handler();
	void update_reflection_x();

	unsigned int extract_lowest_ID();
	void attack();

	virtual void jump() = 0;
	virtual void special() = 0;
	virtual void move(Direction dir) = 0;

	virtual void reset_attributes() = 0;
	virtual void render(void) = 0;
	virtual void update_position(vector<Platform> plat) = 0;
	virtual void physics(vector<Platform> plat) = 0;
	
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
private:
	//My friends
	friend class Toggle;
	friend class Attributes;
	friend class Field_Level;
	friend class Time_Level;
	friend class Night_Level;
	friend class Camera;
	friend class Game;
public:
	void render();
	void update_position(vector<Platform> plat);
	void physics(vector<Platform> plat);
	void move(Direction dir);
	void jump();
	void exhale();
	void special() {}
	void reset_attributes();

	explicit Ball(Assets assets);
	Ball(const Ball&) = delete;
	Ball& operator=(const Ball&) = delete;
};

class Boxy : public Player {
private:
	//My friends
	friend class Toggle;
	friend class Attributes;
	friend class Field_Level;
	friend class Time_Level;
	friend class Night_Level;
	friend class Camera;
	friend class Game;
public:
	void render();
	void update_position(vector<Platform> plat);
	void physics(vector<Platform> plat);
	void move(Direction dir);
	void jump();
	void special() {}
	void reset_attributes();
	
	explicit Boxy(Assets assets);
	Boxy(const Boxy&) = delete;
	Boxy& operator=(const Boxy&) = delete;
};