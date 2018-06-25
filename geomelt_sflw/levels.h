#pragma once

#include "headers.h"
#include "player.h"
#include "assets.h"
#include "complex_shapes.h"
#include "camera.h"

#define GRAVITY 1.0f
#define FRICTION 1.0f

#define TOD_CARDINALITY 6 
#define TRANSITION_RATE_TOD 0.25f

#define MAX_STROKE 4

#define CORNERS 4

#define sm_rnd() (rand() - 0.5f) / 255
#define THICKNESS 8

enum TOD { DAY, AFTERNOON, EVENING, NITE, DNITE, MORNING };

inline void operator++(TOD &ti, int)
{
	const int i = static_cast<int>(ti);
	ti = static_cast<TOD>((i + 1) % TOD_CARDINALITY);
}

class Camera;
class Level;
class Player;

class GradientBG {
public:
	Background TLBG;
	Background TRBG;
	Background LLBG;
	Background LRBG;

	void render();
};

class Level {
public:
	Background background;
	GradientBG gradientBG;
	Quad_ filterBG;

	virtual void render() = 0;
	virtual void gfx_handler(Camera camera) = 0;
	virtual void phys_handler(Assets assets, Camera *camera) = 0;
	void reset();
	void purge_players();

	vector<Platform> platform;

	static map<unsigned int, unique_ptr<Player>> playerMap;

	void add_player(unsigned int joyID, Assets assets);

	Level() {}
	Level(Assets assets);
	virtual ~Level();
};

class Field_Level : public Level {
public:
	Circle_ sun;
	CloudGroup clouds;
	
	void render();
	void gfx_handler(Camera camera);
	void phys_handler(Assets assets, Camera *camera);

	Field_Level() {}
	Field_Level(Assets assets);
	~Field_Level() {}
};

class Night_Level : public Level {
public:
	StarGroup stars;
	Circle_ moon;

	void render();
	void gfx_handler(Camera camera);
	void phys_handler(Assets assets, Camera *camera);

	Night_Level() {}
	Night_Level(Assets assets);
	~Night_Level() {}
};

class Time_Level : public Level {
public:
	TOD time_of_day;
	bool transition;
	Circle_ sun;
	Circle_ moon;
	StarGroup stars; //change opacity during day
	CloudGroup clouds;

	void render();
	void gfx_handler(Camera camera);
	void phys_handler(Assets assets, Camera *camera);

	void transition_handler(Palette_BG pal);
	void transition_to(Color_ *clr);

	Time_Level() {}
	Time_Level(Assets assets);
	~Time_Level() {}
};
