#pragma once

#include "headers.h"

#define M_PI 3.14159265358979323846
constexpr auto TRI_NUM = 50;

class Vec {
public:
	float x, y, z;

	static Vec set_Vec(float xx, float yy, float zz);
};

namespace geomelt
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

		bool isWithin(int x, int y);
		static Boundary setBounds(float t, float b, float l, float r);
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
		~Circle() {}
	};

	class Quad : public Shape {
	public:
		void render();
		Quad() {}
		~Quad() {}
	};
}