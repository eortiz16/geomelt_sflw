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

enum TOD { DAY, AFTERNOON, EVENING, NITE, DNITE, MORNING };

inline void operator++(TOD &ti, int)
{
	const int i = static_cast<int>(ti);
	ti = static_cast<TOD>((i + 1) % TOD_CARDINALITY);
}

class GradientBG {
private:
	Background TLBG;
	Background TRBG;
	Background LLBG;
	Background LRBG;

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
public:
	void render();
};

class Level {
protected:
	Background blackVoid;
	Background background;
	GradientBG gradientBG;
	geomelt::Quad filterBG;
	vector<Platform> platform;
	static map<unsigned int, unique_ptr<Player>> playerMap;

	//My friends
	friend class Game;
	friend class Input;
	friend class CharacterSelect;
	friend class CharacterSelectState;
public:
	virtual void render() = 0;
	virtual void gfx_handler() = 0;
	virtual void phys_handler() = 0;
	void reset_level();
	void purge_players();
	void add_player(unsigned int joyID);

	Level();
	virtual ~Level() {}
};

class Field_Level : public Level {
private:
	vector<unique_ptr<Cloud>> clouds;
	Direction windDirection;
	geomelt::Circle sun;

	// My Friends
	friend class Player;
	friend class Attributes;
public:
	void update_clouds();
	void purge_clouds();
	
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

	// My Friends
	friend class Player;
	friend class Attributes;
public:
	void render();
	void gfx_handler();
	void phys_handler();

	Night_Level();
	~Night_Level() {}
};

class Time_Level : public Level {
private:
	vector<unique_ptr<Cloud>> clouds;
	TOD timeOfDay;
	bool transition;
	geomelt::Circle sun;
	geomelt::Circle moon;
	StarGroup stars; //change opacity during day
	Direction windDirection;
	Palette_BG bg_pal;

	// My Friends
	friend class Player;
	friend class Attributes;
public:
	void update_clouds();
	void purge_clouds();

	void render();
	void gfx_handler();
	void phys_handler();

	void transition_handler();
	void transition_to(geomelt::Color *clr);

	Time_Level();
	~Time_Level() {}
};
