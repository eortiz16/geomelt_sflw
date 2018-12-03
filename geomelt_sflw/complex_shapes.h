#include "headers.h"
#include "assets.h"

#define rnd() (float)rand() / (float)RAND_MAX
constexpr auto MAX_STAR = 40 ;
constexpr auto MAX_SPEED = 5;
constexpr auto MAX_CLOUDS = 100;
constexpr auto SUBCLOUD_SIZE = 3;
constexpr auto CLOUD_START = 50;
constexpr auto CLOUD_RANGE = 200;

#pragma once
class Cloud {
private:
	vector<shared_ptr<medmelt::Circle>> body;
	int speed;
	bool offScreen;
public:
	vector<shared_ptr<medmelt::Circle>> get_body();
	void is_offScreen();
	bool get_offScreen();
	void update(Direction dir);
	void render();
	static Cloud make_cloud(Direction dir); //Factory Method
};

class Star {
private:
	shared_ptr<medmelt::Shape> body;
	float offset;
public:
	void compute_coordinates(int count);
	void set_offset(float val);
	void change_color();
	shared_ptr<medmelt::Shape> get_body();
	Star();
};

class StarGroup {
public:
	Star star[MAX_STAR];
	void render();
	void update();
	StarGroup();
};

class Particles {
public:
	medmelt::Quad s;
	Vec velocity;
};

class TexturedQuad {
public:
	sf::Texture myTexture;
	medmelt::Quad body;
	void set_texture_attributes(sf::Texture asset);
	void render();
};

class RoundCornerBox {
public:
	GLfloat width, height;
	Vec center;
	medmelt::Circle corner[CORNERS];
	medmelt::Quad vRectangle; //vertical
	medmelt::Quad hRectangle; //horizontal
	void set_color(medmelt::Color clr);
	void build();
	void render();

	RoundCornerBox() {}
	RoundCornerBox(Assets assets);
};

class CharSelBox {
public:
	bool occupied;
	TexturedQuad start_icon;
	RoundCornerBox box;
	RoundCornerBox outline;
	GLfloat stroke;
	CharSelBox() { occupied = false; }
};

class Background {
public:
	medmelt::Quad body;
	medmelt::Color color[4];
	bool transition_done[4];
	void render();
	void set_color(medmelt::Color *clr);
	Background() {}
};

class Platform {
public:
	medmelt::Quad body;
	medmelt::Quad outline;
	Color_Set my_color;
};

