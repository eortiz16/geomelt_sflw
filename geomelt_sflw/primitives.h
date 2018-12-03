#pragma once

#include "headers.h"

#define M_PI 3.14159265358979323846
constexpr auto TRI_NUM = 50;

class Vec {
public:
	float x, y, z;
};

namespace medmelt
{
	class Color {
	public:
		float r, g, b, alpha;
		Color() {}
		Color& operator = (const Color &clr)
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
		Color color;
		void render();
		Line();
	};

	class Boundary {
	public:
		float top, bottom, left, right;
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
	};

	class Circle : public Shape {
	public:
		void render();
		Circle() {}
	};

	class Quad : public Shape {
	public:
		void render();
		Quad() {}
	};
}