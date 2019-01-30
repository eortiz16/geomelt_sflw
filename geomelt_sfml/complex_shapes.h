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
public:
	void set_texture_attributes(sf::Texture asset);
	void render();

	TexturedQuad() {}
	~TexturedQuad() {}

	friend class MainMenu;
	friend class MainMenuState;
	friend class CharacterSelect;
	friend class CharacterSelectState;
	friend class LevelSelectState;
	friend class LevelSelect;
	friend class Cursor;
};

class RoundCornerBox {
private:
	GLfloat width, height;
	geomelt::Vec center;
	geomelt::Circle corner[CORNERS];
	geomelt::Quad vRectangle, hRectangle; //vertical, horizontal
public:
	void set_color(geomelt::Color clr);
	void build();
	void render();

	RoundCornerBox();

	friend class CharacterSelect;
	friend class PlayerMap;
};

class CharSelBox {
private:
	bool occupied;
	TexturedQuad start_icon;
	RoundCornerBox box;
	RoundCornerBox outline;
	GLfloat stroke;
public:
	CharSelBox() { occupied = false; }

	friend class CharacterSelect;
	friend class CharacterSelectState;
	friend class AddCharacterCommand;
	friend class RemoveCharacterCommand;
	friend class PlayerMap;
};




