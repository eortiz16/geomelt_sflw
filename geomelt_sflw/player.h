#pragma once

#include "headers.h"
#include "assets.h"
#include "complex_shapes.h"
#include "levels.h"
#include "controller.h"

#define MOVE_PARAM_X 15.0f
#define MOVE_PARAM_Y 5.0f
#define JUMP_PARAM 25.0f

class Level;

enum PlayerState { ALIVE, DEAD, ELIMINATED };
enum SelectColor{NEXT, PREV, RANDOM};
class buttonMapping;

class Attributes {
public:
	float health;
	int lifeCount;
	PlayerState lifeState;
	Attributes();
};

class Toggle {
public:
	bool initDeath;
	bool attacking;
	bool walk_toggle;
	bool on_ground;

	sf::Clock deathTimer;
	sf::Clock attackTimer;
	Toggle();
};

class Player {
private:
	static unsigned int count;
	static vector<int> availableIDs;
public:
	unsigned int get_count();
	unsigned int myID;

	Toggle toggle;

	Attributes stats;
	Vec	velocity;
	bool created;

	//ButtonMapping buttonMapping;
	void read_buttons(unsigned int button);
	void read_axes(unsigned int joyID);
	
	void simple_update();
	void simple_update_menu();

	CharColorOptions myColor;

	Shape_ *body;
	Shape_ *outline;
	Shape_ *reflection;
	Quad_ arm;
	Quad_ armOutline;
	Circle_ eye;

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
	~Ball() {}
};

class Boxy : public Player {
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
	~Boxy() {}
};