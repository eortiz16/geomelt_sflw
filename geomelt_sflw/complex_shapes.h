#include "headers.h"
#include "assets.h"

#define MAX_STAR 40 
#define MAX_SPEED 2
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_CLOUD 10
#define CLOUD_GROUP 3
#define CLOUD_START 50
#define CLOUD_RANGE 200

#pragma once
class Cloud {
public:
	Circle_ body[3];
	int speed;
	void update(Direction dir);
	void render();
	void reset_cloud(Direction dir);
};

class CloudGroup {
public:
	Direction direction;
	Cloud cloud[MAX_CLOUD];
	void render();
	void update();
	CloudGroup();
};

class Star {
public:
	Circle_ body;
	float offset;
	void compute_coordinates(int count);
	void change_color();
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
	Quad_ s;
	Vec velocity;
};

class RoundCornerBox {
public:
	GLfloat width, height;
	Vec center;
	Circle_ corner[CORNERS];
	Quad_ vRectangle; //vertical
	Quad_ hRectangle; //horizontal
	void set_color(Color_ clr);
	void build();
	void render();
	RoundCornerBox();
};

class CharSelBox {
public:
	RoundCornerBox box;
	RoundCornerBox outline;
	GLfloat stroke;
};

class Background {
public:
	Quad_ body;
	Color_ color[4];
	bool transition_done[4];
	void render();
	void set_color(Color_ *clr);
	Background() {}
};

class Platform {
public:
	Quad_ body;
	Quad_ outline;
	Color_Set my_color;
};

class TexturedQuad {
public:
	sf::Texture myTexture;
	Quad_ body;
	void set_texture_attributes(sf::Texture asset);
	void render();
};