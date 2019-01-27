#pragma once

#include "headers.h"
#include "player.h"
#include "assets.h"
#include "complex_shapes.h"
#include "camera.h"

#define sm_rnd() (rand() - 0.5f) / 255
constexpr auto GRAVITY = 1.0f;
constexpr auto FRICTION = 1.0f;
constexpr auto TOD_CARDINALITY = 6 ;
constexpr auto TRANSITION_RATE_TOD = 0.25f;
constexpr auto MAX_STROKE = 4;
constexpr auto THICKNESS = 8;

class Game;
class Camera;
class Player;

enum Lvl { FIELD, NIGHT, TIME };
enum TOD { DAY, AFTERNOON, EVENING, NITE, DNITE, MORNING };

inline void operator++(TOD &ti, int)
{
	const int i = static_cast<int>(ti);
	ti = static_cast<TOD>((i + 1) % TOD_CARDINALITY);
}

class Platform {
private:
	geomelt::Quad body;
	geomelt::Quad outline;
	Color_Set my_color;

	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class PlatformGroup;
};

class PlatformGroup {
private:
	vector<Platform> platforms;
public:
	void render();

	PlatformGroup();
	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class Player;
	friend class Ball;
	friend class Boxy;
};

class Level {
protected:
	Background blackVoid;
	Background background;
	PlatformGroup platforms;
	static map<unsigned int, unique_ptr<Player>> playerMap;

	friend class CharacterSelectState;
	friend class LevelState;
	friend class JumpCommand;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
	friend class StopCommand;
	friend class ChangeCharacterCommand;
	friend class AddCharacterCommand;
	friend class PrevColorCommand;
	friend class NextColorCommand;
public:
	virtual void render() = 0;
	virtual void gfx_handler() = 0;
	virtual void phys_handler() = 0;
	void reset_level();
	void purge_players();
	void add_player(unsigned int joyID);
	static unique_ptr<Level> make(Lvl lvl); //Factory Method

	Level();
	virtual ~Level() {}
};

class Field_Level : public Level {
private:
	CloudGroup clouds;
	geomelt::Circle sun;
public:
	void render();
	void gfx_handler();
	void phys_handler();

	Field_Level();
	~Field_Level() {}
};

class Night_Level : public Level {
private:
	StarGroup stars;
	geomelt::Circle moon;
public:
	void render();
	void gfx_handler();
	void phys_handler();

	Night_Level();
	~Night_Level() {}
};

class Time_Level : public Level {
private:
	TOD timeOfDay;
	bool transition;
	geomelt::Circle sun;
	geomelt::Circle moon;
	StarGroup stars; //change opacity during day
	CloudGroup clouds;
	Palette_BG bg_pal;
public:
	void render();
	void gfx_handler();
	void phys_handler();
	void transition_handler();
	void transition_to(geomelt::Color *clr);

	Time_Level();
	~Time_Level() {}
};
