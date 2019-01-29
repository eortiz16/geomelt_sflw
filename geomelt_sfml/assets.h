#pragma once

#include "headers.h"
#include "generic.h"
#include "primitives.h"

constexpr auto CLR_OPT_CARDINALITY = 8;

class ColorSet {
private:
	geomelt::Color body;
	geomelt::Color outline;

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class CharacterColorSet;
	friend class PlatformPalette;
	friend class CharacterPalette;
	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class PlayerMap;
	friend class PlatformGroup;
public:
	ColorSet() {}
	ColorSet& operator = (const ColorSet &clr);
	ColorSet(geomelt::Color, geomelt::Color);
};

class CharacterColorSet : public ColorSet {
private:
	geomelt::Color reflection;
	
	friend class CharacterPalette;
	friend class Player;
	friend class PlayerMap;
public:
	CharacterColorSet() {}
	CharacterColorSet& operator = (const CharacterColorSet &clr);
	CharacterColorSet(geomelt::Color, geomelt::Color, geomelt::Color);
};

class Palette {
public:
	const static geomelt::Color sun;
	const static geomelt::Color moon;
	const static geomelt::Color platform;
	const static geomelt::Color black;
	const static geomelt::Color grey;
	const static geomelt::Color darkGrey;
	const static geomelt::Color lightGrey;
	const static geomelt::Color white;
	const static geomelt::Color red;
	const static geomelt::Color darkRed;
	const static geomelt::Color green;
	const static geomelt::Color darkGreen;
	const static geomelt::Color blue;
	const static geomelt::Color darkBlue;
};

class BackgroundPalette {
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
	friend class CharacterSelect;
	friend class SceneryGroup;
	friend class FieldScenery;
	friend class NightScenery;
	friend class TimeScenery;
	friend class LevelSelect;
public:
	BackgroundPalette();
};

class PlatformPalette {
public:
	const static ColorSet grass; 
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

class CharacterPalette {
public:
	const static map <CharColorOptions, CharacterColorSet> colors;
	static map <CharColorOptions, CharacterColorSet> init();
	CharacterPalette() {}
};

class Textures {
public:
	const static sf::Texture field;
	const static sf::Texture night;
	const static sf::Texture time;
	const static sf::Texture polluted;
	const static sf::Texture title;
	const static sf::Texture play;
	const static sf::Texture playSelected;
	const static sf::Texture options;
	const static sf::Texture optionsSelected;
	const static sf::Texture exit;
	const static sf::Texture exitSelected;
	const static sf::Texture button_A;
	const static sf::Texture button_B;
	const static sf::Texture button_X;
	const static sf::Texture button_Y;
	const static sf::Texture button_LB;
	const static sf::Texture button_RB;
	const static sf::Texture button_LS;
	const static sf::Texture button_RS;
	const static sf::Texture button_LT;
	const static sf::Texture button_RT;
	const static sf::Texture button_Back;
	const static sf::Texture button_Start;
	const static sf::Texture button_Left;
	const static sf::Texture button_Right;
	const static sf::Texture button_Down;
	const static sf::Texture button_Up;

	static sf::Texture setTexture(string);
};

class Assets {
public:
	static Textures textures;
	static Palette palette;
	static BackgroundPalette backgroundPalette;
	static CharacterPalette characterPalette;
	static PlatformPalette platformPalette;
};
