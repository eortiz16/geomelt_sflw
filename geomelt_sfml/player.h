#pragma once

#include "headers.h"
#include "assets.h"
#include "complex_shapes.h"
#include "levels.h"
#include "command.h"
#include <set>

constexpr auto JUMP_PARAM = 25.0f;
enum PlayerState { ALIVE, DEAD, ELIMINATED };
enum SelectColor{NEXT, PREV, RANDOM};

class PlatformGroup;

class Attributes {
private:
	float health;
	int lifeCount;
	PlayerState lifeState;
public:
	Attributes();

	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class PlayerMap;
	friend class Camera;
};

class Toggle {
private:
	bool initDeath;
	bool attacking;
	bool walking;
	bool on_ground;
	sf::Clock deathTimer;
	sf::Clock attackTimer;
public:
	Toggle();

	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class PlayerMap;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
	friend class StopCommand;
};

class Player {
protected:
	static vector<int> availableIDs;
	float weight;
	geomelt::Vec velocity;
	float move_param_x; // speed
	float move_param_y; // jump
	int JUMP_MAX;
	int jumpCount;
	Direction direction;
	unsigned int myID;
	Toggle toggle;
	Attributes stats;
	unique_ptr<geomelt::Shape> body;
	unique_ptr<geomelt::Shape> outline;
	unique_ptr<geomelt::Shape> reflection;
	geomelt::Quad arm;
	geomelt::Quad armOutline;
	geomelt::Circle eye;
	CharColorOptions myColor;
	CharacterColorSet mColor;
	float mDimension;
public:
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
	virtual void render();
	virtual void update(PlatformGroup plat);
	virtual void physics(PlatformGroup plat);
	
	Player& operator = (const CharacterColorSet &clr);
	explicit Player();
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	virtual ~Player();

	friend class PlayerMap;
	friend class Camera;
};

class Ball : public Player {
public:
	void update(PlatformGroup plat);
	void jump();
	void exhale();
	void special() {}

	explicit Ball();
	Ball(const Ball&) = delete;
	Ball& operator=(const Ball&) = delete;
	~Ball() {}
};

class Boxy : public Player {
public:
	void update(PlatformGroup plat);
	void jump();
	void special() {}
	
	explicit Boxy();
	Boxy(const Boxy&) = delete;
	Boxy& operator=(const Boxy&) = delete;
	~Boxy() {}
};

class PlayerMap {
private:
	static map<unsigned int, unique_ptr<Player>> _map;
public:
	void add(unsigned int id);
	void purge(unsigned int id);
	void transform(unsigned int id);
	void change_color(unsigned int id, SelectColor dir);
	void attack(unsigned int id);
	void jump(unsigned int id);
	void move(unsigned int id, Direction dir);
	void stop(unsigned int id);
	void clear();
	unsigned int size();
	void render();
	void options_render(vector<CharSelBox> selectBox);
	void phys_handler(PlatformGroup plat);
	void reset();

	friend class Camera;
};
