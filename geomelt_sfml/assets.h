#pragma once

#include "headers.h"
#include "primitives.h"

constexpr auto CLR_OPT_CARDINALITY = 8;

class ColorSet {
private:
	Color body;
	Color outline;

	friend class Level;
	friend class CharacterColorSet;
	friend class PlatformPalette;
	friend class CharacterPalette;
	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class PlayerMap;
	friend class Platform;
	friend class PlatformGroup;
public:
	ColorSet() {}
	ColorSet& operator = (const ColorSet &clr);
	ColorSet(Color, Color);
};

class CharacterColorSet : public ColorSet {
private:
	Color reflection;
	
	friend class CharacterPalette;
	friend class Player;
	friend class Ball;
	friend class Boxy;
	friend class PlayerMap;
public:
	CharacterColorSet() {}
	CharacterColorSet& operator = (const CharacterColorSet &clr);
	CharacterColorSet(Color, Color, Color);
};

class Palette {
public:
	const static Color sun;
	const static Color moon;
	const static Color platform;
	const static Color black;
	const static Color grey;
	const static Color darkGrey;
	const static Color lightGrey;
	const static Color white;
	const static Color red;
	const static Color darkRed;
	const static Color green;
	const static Color darkGreen;
	const static Color blue;
	const static Color darkBlue;
};

class BackgroundPalette {
private:
	vector<Color> day;
	vector<Color> afternoon;
	vector<Color> evening;
	vector<Color> night;
	vector<Color> dark_night;
	vector<Color> morning;
	vector<Color> overcast;
	vector<Color> black;

	friend class Level;
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
	const static sf::Texture resume;
	const static sf::Texture resumeSelected;
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
