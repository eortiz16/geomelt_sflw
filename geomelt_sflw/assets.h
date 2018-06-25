#pragma once

#include "headers.h"
#include "generic.h"
#include "primitives.h"

#define CLR_OPT_CARDINALITY 8

class Color_Set {
public:
	Color_ body;
	Color_ outline;

	Color_Set& operator = (const Color_Set &clr)
	{
		body = clr.body;
		outline = clr.outline;
		return *this;
	}
};

class Char_Color_Set : public Color_Set {
public:
	Color_ reflection;

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
	Color_ sun;
	Color_ moon;
	Color_ platform;
	Color_ black;
	Color_ grey;
	Color_ darkGrey;
	Color_ lightGrey;
	Color_ white;
	Color_ red;
	Color_ darkRed;
	Color_ green;
	Color_ darkGreen;
	Color_ blue;
	Color_ darkBlue;
	Palette();
};

class Palette_BG {
public:
	Color_ day[CORNERS];
	Color_ afternoon[CORNERS];
	Color_ evening[CORNERS];
	Color_ night[CORNERS];
	Color_ dark_night[CORNERS];
	Color_ morning[CORNERS];
	Color_ overcast[CORNERS];
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

	Textures();
};

class Assets {
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
