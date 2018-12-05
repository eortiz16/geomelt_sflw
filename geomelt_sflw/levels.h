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
protected:
	Background background;
	GradientBG gradientBG;
	medmelt::Quad filterBG;
	vector<Platform> platform;
	static map<unsigned int, unique_ptr<Player>> playerMap;
public:
	virtual void render() = 0;
	virtual void gfx_handler(Camera camera) = 0;
	virtual void phys_handler(Assets assets, Camera *camera) = 0;
	void reset_level();
	void purge_players();
	void add_player(unsigned int joyID, Assets assets);

	//My friends
	friend class Game;
	friend class Input;
	friend class CharacterSelect;

	Level() {}
	Level(Assets assets);
	virtual ~Level();
};

class Field_Level : public Level {
private:
	vector<unique_ptr<Cloud>> clouds;
	Direction windDirection;
	medmelt::Circle sun;

	// My Friends
	friend class Player;
	friend class Attributes;
public:
	void update_clouds();
	void purge_clouds();
	
	void render();
	void gfx_handler(Camera camera);
	void phys_handler(Assets assets, Camera *camera);

	Field_Level() {}
	Field_Level(Assets assets);
	~Field_Level() {}
};

class Night_Level : public Level {
private:
	StarGroup stars;
	medmelt::Circle moon;

	// My Friends
	friend class Player;
	friend class Attributes;
public:
	void render();
	void gfx_handler(Camera camera);
	void phys_handler(Assets assets, Camera *camera);

	Night_Level() {}
	Night_Level(Assets assets);
	~Night_Level() {}
};

class Time_Level : public Level {
private:
	vector<unique_ptr<Cloud>> clouds;
	TOD timeOfDay;
	bool transition;
	medmelt::Circle sun;
	medmelt::Circle moon;
	StarGroup stars; //change opacity during day
	Direction windDirection;

	// My Friends
	friend class Player;
	friend class Attributes;
public:
	void update_clouds();
	void purge_clouds();

	void render();
	void gfx_handler(Camera camera);
	void phys_handler(Assets assets, Camera *camera);

	void transition_handler(Palette_BG pal);
	void transition_to(medmelt::Color *clr);

	Time_Level() {}
	Time_Level(Assets assets);
	~Time_Level() {}
};
