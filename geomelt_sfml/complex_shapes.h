#include "headers.h"
#include "assets.h"

#define rnd() (float)rand() / (float)RAND_MAX
constexpr auto MAX_STAR = 80 ;
constexpr auto MAX_SPEED = 5;
constexpr auto MAX_CLOUDS = 1;
constexpr auto SUBCLOUD_SIZE = 3;
constexpr auto CLOUD_START = 50;
constexpr auto CLOUD_RANGE = 200;

#pragma once
class Cloud {
private:
	vector<Circle> body;
	int speed;
	bool offScreen;
public:
	void update(Direction dir);
	void render();
	static Cloud make_cloud(Direction dir); //Factory Method

	Cloud();
	Cloud(Circle, int, Direction);
	~Cloud();
	friend class Clouds;
};

class Star {
private:
	unique_ptr<Shape> body;
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
	Quad s;
	Vec velocity;
};

class TexturedQuad : public Shape {
private:
	sf::Texture myTexture;
public:
	void set_texture_attributes(sf::Texture asset);
	void render();

	TexturedQuad(sf::Texture texture, float w, float h, Vec v);
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
	Vec center;
	Circle corner[CORNERS];
	Quad vRectangle, hRectangle; //vertical, horizontal
public:
	void set_color(Color clr);
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




