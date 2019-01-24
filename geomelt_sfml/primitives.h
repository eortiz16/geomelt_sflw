#pragma once

#include "headers.h"

#define M_PI 3.14159265358979323846
constexpr auto TRI_NUM = 50;

namespace geomelt
{
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
		friend class MainMenu;
		friend class Player;
		friend class Ball;
		friend class Boxy;
		friend class LevelSelect;
		friend class CharacterSelect;
	};

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
		friend class Field_Level;
		friend class Night_Level;
		friend class Time_Level;
		friend class Game;
		friend class GFXNet;
	};

	class Shape {
	public:
		float width, height, radius;
		Boundary boundary;
		Color color;
		Vec center;
	
		void boundary_assignment();
		virtual void render() = 0; // <--- Abstract class
		Shape() {}
		~Shape() {}

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
		Circle() {}
		~Circle() {}

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
		Quad(Vec v, float w, float h) { center.x = v.x; center.y = v.y; width = w; height = h; };
		~Quad() {}

		friend class Star;
		friend class Cloud;
		friend class RoundCornerBox;
		friend class Background;
		friend class LevelSelect;
		friend class CharacterSelect;
	};
}