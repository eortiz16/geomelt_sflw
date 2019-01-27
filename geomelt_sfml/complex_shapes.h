#include "headers.h"
#include "assets.h"

#define rnd() (float)rand() / (float)RAND_MAX
constexpr auto MAX_STAR = 80 ;
constexpr auto MAX_SPEED = 5;
constexpr auto MAX_CLOUDS = 20;
constexpr auto SUBCLOUD_SIZE = 3;
constexpr auto CLOUD_START = 50;
constexpr auto CLOUD_RANGE = 200;

#pragma once
class Cloud {
private:
	vector<unique_ptr<geomelt::Circle>> body;
	int speed;
	bool offScreen;
	friend class Field_Level;
	friend class Time_Level;
public:
	void update(Direction dir);
	void render();
	static Cloud make_cloud(Direction dir); //Factory Method

	friend class CloudGroup;
};

class CloudGroup {
private:
	vector<Cloud> clouds;
	Direction windDirection;
public:
	void update();
	void purge();
	void render();

	CloudGroup();
};

class Star {
private:
	unique_ptr<geomelt::Shape> body;
	friend class StarGroup;
public:
	void change_color();
	Star();
	Star(unsigned int seed);
};

class StarGroup {
private:
	vector<Star> star;
	friend class Night_Level;
	friend class Time_Level;
public:
	void render();
	void update();
	StarGroup();
};

class Particles {
private:
	geomelt::Quad s;
	geomelt::Vec velocity;
};

class TexturedQuad : public geomelt::Shape {
private:
	sf::Texture myTexture;

	friend class MainMenu;
	friend class MainMenuState;
	friend class CharacterSelect;
	friend class CharacterSelectState;
	friend class LevelSelectState;
	friend class LevelSelect;
	friend class Cursor;
public:
	void set_texture_attributes(sf::Texture asset);
	void render();

	TexturedQuad() {}
	~TexturedQuad() {}
};

class RoundCornerBox {
private:
	GLfloat width, height;
	geomelt::Vec center;
	geomelt::Circle corner[CORNERS];
	geomelt::Quad vRectangle, hRectangle; //vertical, horizontal

	friend class CharacterSelect;
public:
	void set_color(geomelt::Color clr);
	void build();
	void render();

	RoundCornerBox();
};

class CharSelBox {
private:
	bool occupied;
	TexturedQuad start_icon;
	RoundCornerBox box;
	RoundCornerBox outline;
	GLfloat stroke;

	friend class CharacterSelect;
	friend class CharacterSelectState;
	friend class AddCharacterCommand;
public:
	CharSelBox() { occupied = false; }
};

class Background {
private:
	geomelt::Quad body;
	geomelt::Color color[4];
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
	void set_color(geomelt::Color *clr);
	Background() {}
};


