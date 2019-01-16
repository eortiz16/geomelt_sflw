#include "headers.h"
#include "assets.h"

#define rnd() (float)rand() / (float)RAND_MAX
constexpr auto MAX_STAR = 40 ;
constexpr auto MAX_SPEED = 5;
constexpr auto MAX_CLOUDS = 20;
constexpr auto SUBCLOUD_SIZE = 3;
constexpr auto CLOUD_START = 50;
constexpr auto CLOUD_RANGE = 200;

#pragma once
class Cloud {
private:
	vector<unique_ptr<medmelt::Circle>> body;
	int speed;
	bool offScreen;
	friend class Field_Level;
	friend class Time_Level;
public:
	void update(Direction dir);
	void render();
	static Cloud make_cloud(Direction dir); //Factory Method
};

class Star {
private:
	unique_ptr<medmelt::Shape> body;
	float offset;
	friend class StarGroup;
public:
	void compute_coordinates(int count);
	void set_offset(float val);
	void change_color();
	Star();
};

class StarGroup {
private:
	Star star[MAX_STAR];
	friend class Night_Level;
	friend class Time_Level;
public:
	void render();
	void update();
	StarGroup();
};

class Particles {
private:
	medmelt::Quad s;
	Vec velocity;
};

class TexturedQuad {
private:
	sf::Texture myTexture;
	medmelt::Quad body;

	friend class MainMenu;
	friend class CharacterSelect;
	friend class LevelSelect;
public:
	void set_texture_attributes(sf::Texture asset);
	void render();
};

class RoundCornerBox {
private:
	GLfloat width, height;
	Vec center;
	medmelt::Circle corner[CORNERS];
	medmelt::Quad vRectangle, hRectangle; //vertical, horizontal

	friend class CharacterSelect;
public:
	void set_color(medmelt::Color clr);
	void build();
	void render();

	RoundCornerBox() {}
	RoundCornerBox(Assets assets);
};

class CharSelBox {
private:
	bool occupied;
	TexturedQuad start_icon;
	RoundCornerBox box;
	RoundCornerBox outline;
	GLfloat stroke;

	friend class CharacterSelect;
public:
	CharSelBox() { occupied = false; }
};

class Background {
private:
	medmelt::Quad body;
	medmelt::Color color[4];
	bool transition_done[4];

	friend class GradientBG;
	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class LevelSelect;
	friend class CharacterSelect;
public:
	void render();
	void set_color(medmelt::Color *clr);
	Background() {}
};

class Platform {
private:
	medmelt::Quad body;
	medmelt::Quad outline;
	Color_Set my_color;
	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
};

