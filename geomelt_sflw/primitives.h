#pragma once

#include "headers.h"

#define M_PI 3.14159265358979323846
#define TRI_NUM 50

class Vec {
public:
	float x, y, z;
};

class Color_ {
public:
	float r, g, b, alpha;
	Color_() {}
	Color_& operator = (const Color_ &clr)
	{
		r = clr.r;
		g = clr.g;
		b = clr.b;
		alpha = clr.alpha;
		return *this;
	}
};

class Line {
public:
	float width;
	Vec from, to;
	Color_ color;
	void render();
	Line();
};

class Boundary_ {
public:
	float top, bottom, left, right;
};

class Shape_ {
public:
	float width, height, radius;
	Boundary_ boundary;
	Color_ color;
	Vec center;
	void boundary_assignment();
	virtual void render() = 0;
	Shape_() {}
};

class Circle_ : public Shape_ {
public:
	void render();
	Circle_() {}
};

class Quad_ : public Shape_ {
public:
	void render();
	Quad_() {}
};