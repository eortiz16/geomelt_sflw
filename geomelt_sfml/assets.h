#pragma once

#include "headers.h"
#include "generic.h"
#include "primitives.h"

constexpr auto CLR_OPT_CARDINALITY = 8;

class Color_Set {
private:
	geomelt::Color body;
	geomelt::Color outline;

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class Char_Color_Set;
	friend class Palette_PLAT;
	friend class Palette_CHAR;
	friend class Player;
	friend class Ball;
	friend class Boxy;
public:
	Color_Set& operator = (const Color_Set &clr)
	{
		body = clr.body;
		outline = clr.outline;
		return *this;
	}
};

class Char_Color_Set : public Color_Set {
private:
	geomelt::Color reflection;
	
	friend class Palette_CHAR;
	friend class Player;
	friend class Ball;
	friend class Boxy;
public:
	Char_Color_Set& operator = (const Char_Color_Set &clr)
	{
		body = clr.body;
		outline = clr.outline;
		reflection = clr.reflection;
		return *this;
	}
};

class Palette {
private:
	geomelt::Color sun;
	geomelt::Color moon;
	geomelt::Color platform;
	geomelt::Color black;
	geomelt::Color grey;
	geomelt::Color darkGrey;
	geomelt::Color lightGrey;
	geomelt::Color white;
	geomelt::Color red;
	geomelt::Color darkRed;
	geomelt::Color green;
	geomelt::Color darkGreen;
	geomelt::Color blue;
	geomelt::Color darkBlue;

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class Palette_PLAT;
	friend class Palette_CHAR;
	friend class Ball;
	friend class Boxy;
	friend class CharacterSelect;
	friend class LevelSelect;
public:
	Palette();
};

class Palette_BG {
private:
	geomelt::Color day[CORNERS];
	geomelt::Color afternoon[CORNERS];
	geomelt::Color evening[CORNERS];
	geomelt::Color night[CORNERS];
	geomelt::Color dark_night[CORNERS];
	geomelt::Color morning[CORNERS];
	geomelt::Color overcast[CORNERS];
	geomelt::Color black[CORNERS];

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
public:
	Palette_BG();
};

class Palette_PLAT {
private:
	Color_Set grass;

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
public:
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
private:
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

	friend class Player;
	friend class CharacterSelect;
	friend class Level;
public:
	Palette_CHAR();
};

class Textures {
private:
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

	friend class MainMenu;
	friend class MainMenuState;
	friend class CharacterSelect;
	friend class LevelSelect;
public:
	Textures();
};

class Assets {
private:
	static sf::Font font;
	static sf::Texture backgroundTexture;
	static sf::Texture texture;
	static Textures textures;
	static sf::Sprite background;
	static Palette palette;
	static Palette_BG backgroundPalette;
	static Palette_CHAR characterPalette;
	static Palette_PLAT platformPalette;

	friend class MainMenu;
	friend class CharacterSelect;
	friend class LevelSelect;
	friend class Level;
	friend class MainMenuState;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class Ball;
	friend class Boxy;
public:
	Assets();
	~Assets() {}
};
