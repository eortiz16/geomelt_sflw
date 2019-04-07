#pragma once

#include "headers.h"

#define M_PI 3.14159265358979323846
constexpr auto TRI_NUM = 50;

class Vec {
public:
	float x, y, z;
	
	Vec() {}
	Vec(float, float, float);

	friend class Line;
	friend class Shape;
	friend class Quad;
	friend class Circle;
	friend class TexturedQuad;
	friend class Star;
	friend class Cloud;
	friend class RoundCornerBox;
	friend class Background;
	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class Menu;
	friend class MainMenu;
	friend class LevelSelect;
	friend class CharacterSelect;
	friend class Level;
	friend class Pause;
	friend class Camera;
	friend class Cloud;
	friend class Player;
};

class Color {
public:
	uint8_t r, g, b, alpha;

	Color() {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	Color& operator = (const Color &clr);

	friend class Line;
	friend class Shape;
	friend class Quad;
	friend class Circle;
	friend class Star;
	friend class Cloud;
	friend class RoundCornerBox;
	friend class Background;
	friend class LevelSelect;
};

class Line {
private:
	float width;
	Vec from, to;
	Color color;
public:
	void render();
	Line();
};

class Boundary {
public:
	float top, bottom, left, right;

	bool isWithin(int x, int y); /* For mouse detection */
	bool isWithin(Boundary); /* one boundary within another? */
	bool isWithin(Boundary, float center);/* for use with player */

	Boundary() {}
	Boundary(float t, float b, float l, float r);

	friend class Shape;
	friend class Quad;
	friend class Circle;
	friend class Camera;
	friend class CharacterSelect;
	friend class RoundCornerBox;
	friend class Platform;
	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class Level;
	friend class GFXNet;
};

class Shape {
public:
	float width, height, radius;
	Boundary boundary;
	Color color;
	Vec center;
	
	void boundary_assignment();
	virtual void render() = 0;
	Shape() {}
	Shape(float w, float h, float r, Vec v, Color clr = Color(0, 0, 0, 255));
	virtual ~Shape() {}

	friend class Star;
	friend class Cloud;
	friend class RoundCornerBox;
	friend class Background;
	friend class LevelSelect;
	friend class CharacterSelect;
};

class Circle : public Shape {
public:
	virtual void render();
	explicit Circle() {}
	Circle(float w, float h, float r, Color clr, Vec v) : Shape(w, h, r, v, clr) {}
	Circle(float r, Color clr, Vec v) : Shape(0, 0, r, v, clr) {}
	virtual ~Circle() {}

	friend class Star;
	friend class Cloud;
	friend class RoundCornerBox;
	friend class Background;
	friend class LevelSelect;
	friend class CharacterSelect;
};

class Quad : public Shape {
public:
	void render();
	Quad() {}
	Quad(float w, float h, float r, Color clr, Vec v) : Shape(w, h, r, v, clr) {}
	virtual ~Quad() {}

	friend class Star;
	friend class Cloud;
	friend class RoundCornerBox;
	friend class Background;
	friend class LevelSelect;
	friend class CharacterSelect;
};
