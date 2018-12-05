#pragma once

#include "headers.h"
#include "generic.h"
#include "primitives.h"

constexpr auto CLR_OPT_CARDINALITY = 8;

class Color_Set {
public:
	medmelt::Color body;
	medmelt::Color outline;

	Color_Set& operator = (const Color_Set &clr)
	{
		body = clr.body;
		outline = clr.outline;
		return *this;
	}
};

class Char_Color_Set : public Color_Set {
public:
	medmelt::Color reflection;

	Char_Color_Set& operator = (const Char_Color_Set &clr)
	{
		body = clr.body;
		outline = clr.outline;
		reflection = clr.reflection;
		return *this;
	}
};

class Palette {
public:
	medmelt::Color sun;
	medmelt::Color moon;
	medmelt::Color platform;
	medmelt::Color black;
	medmelt::Color grey;
	medmelt::Color darkGrey;
	medmelt::Color lightGrey;
	medmelt::Color white;
	medmelt::Color red;
	medmelt::Color darkRed;
	medmelt::Color green;
	medmelt::Color darkGreen;
	medmelt::Color blue;
	medmelt::Color darkBlue;
	Palette();
};

class Palette_BG {
public:
	medmelt::Color day[CORNERS];
	medmelt::Color afternoon[CORNERS];
	medmelt::Color evening[CORNERS];
	medmelt::Color night[CORNERS];
	medmelt::Color dark_night[CORNERS];
	medmelt::Color morning[CORNERS];
	medmelt::Color overcast[CORNERS];
	medmelt::Color black[CORNERS];
	Palette_BG();
};

class Palette_PLAT {
public:
	Color_Set grass;
	Palette_PLAT(Palette pal);
};

enum CharColorOptions { RED, GREEN, BLUE, YELLOW, PINK, PURPLE, WHITE, BLACK };

inline void operator++(CharColorOptions &ti, int)
{
	const int i = static_cast<int>(ti);
	ti = static_cast<CharColorOptions>((i + 1) % CLR_OPT_CARDINALITY);
}

inline void operator--(CharColorOptions &ti, int)
{
	const int i = static_cast<int>(ti);
	ti = static_cast<CharColorOptions>((i - 1) % CLR_OPT_CARDINALITY);
	if (ti < 0)
		ti = static_cast<CharColorOptions> (CLR_OPT_CARDINALITY - 1);
}

class Palette_CHAR {
public:
	//IF add color, change cardinality of this set
	Char_Color_Set red;
	Char_Color_Set green;
	Char_Color_Set blue;
	Char_Color_Set yellow;
	Char_Color_Set pink;
	Char_Color_Set purple;
	Char_Color_Set white;
	Char_Color_Set black;

	map <CharColorOptions, Char_Color_Set> traverse_colors;

	Palette_CHAR();
};

class Textures {
public:
	sf::Texture field;
	sf::Texture night;
	sf::Texture time;
	sf::Texture polluted;

	sf::Texture title;
	sf::Texture play;
	sf::Texture playSelected;
	sf::Texture options;
	sf::Texture optionsSelected;
	sf::Texture exit;
	sf::Texture exitSelected;

	sf::Texture button_A;
	sf::Texture button_B;
	sf::Texture button_X;
	sf::Texture button_Y;
	sf::Texture button_LB;
	sf::Texture button_RB;
	sf::Texture button_LS;
	sf::Texture button_RS;
	sf::Texture button_LT;
	sf::Texture button_RT;
	sf::Texture button_Back;
	sf::Texture button_Start;
	sf::Texture button_Left;
	sf::Texture button_Right;
	sf::Texture button_Down;
	sf::Texture button_Up;

	Textures();
};

class Assets {
private:

public:
	sf::Font font;
	
	sf::Texture backgroundTexture;
	sf::Texture texture;

	Textures textures;
	Sprite background;

	Palette palette;
	Palette_BG backgroundPalette;
	Palette_CHAR characterPalette;
	Palette_PLAT platformPalette = Palette_PLAT(palette);

	Assets();
	~Assets();
};
