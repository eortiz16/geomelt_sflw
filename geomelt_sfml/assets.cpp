#include "assets.h"

Textures Assets::textures;
Palette Assets::palette;
BackgroundPalette Assets::backgroundPalette;
CharacterPalette Assets::characterPalette;
const geomelt::Color Palette::sun = geomelt::Color(255, 255, 100, 255);
const geomelt::Color Palette::moon = geomelt::Color(225, 225, 215, 255);
const geomelt::Color Palette::platform = geomelt::Color(99, 160, 0, 255);
const geomelt::Color Palette::black = geomelt::Color(0, 0, 0, 255);
const geomelt::Color Palette::grey = geomelt::Color(150, 150, 150, 255);
const geomelt::Color Palette::darkGrey = geomelt::Color(25, 25, 25, 255);
const geomelt::Color Palette::lightGrey = geomelt::Color(210, 210, 210, 255);
const geomelt::Color Palette::white = geomelt::Color(255, 255, 255, 255);
const geomelt::Color Palette::red = geomelt::Color(255, 0, 0, 255);
const geomelt::Color Palette::darkRed = geomelt::Color(150, 0, 0, 255);
const geomelt::Color Palette::green = geomelt::Color(0, 255, 0, 255);
const geomelt::Color Palette::darkGreen = geomelt::Color(0, 150, 0, 255);
const geomelt::Color Palette::blue = geomelt::Color(0, 0, 255, 255);
const geomelt::Color Palette::darkBlue = geomelt::Color(0, 0, 150, 255);
const ColorSet PlatformPalette::grass = ColorSet(Palette::platform, Palette::black);
const map<CharColorOptions, CharacterColorSet> CharacterPalette::colors = CharacterPalette::init();
const sf::Texture Textures::field = setTexture("resources/textures/field.png");
const sf::Texture Textures::night = setTexture("resources/textures/night.png");
const sf::Texture Textures::time = setTexture("resources/textures/time.png");
const sf::Texture Textures::title = setTexture("resources/textures/title.png");
const sf::Texture Textures::play = setTexture("resources/textures/play_u.png");
const sf::Texture Textures::playSelected = setTexture("resources/textures/play_s.png");
const sf::Texture Textures::options = setTexture("resources/textures/options_u.png");
const sf::Texture Textures::optionsSelected = setTexture("resources/textures/options_s.png");
const sf::Texture Textures::exit = setTexture("resources/textures/exit_u.png");
const sf::Texture Textures::exitSelected = setTexture("resources/textures/exit_s.png");
const sf::Texture Textures::resume = setTexture("resources/textures/resume_u.png");
const sf::Texture Textures::resumeSelected = setTexture("resources/textures/resume_s.png");
const sf::Texture Textures::button_A = setTexture("resources/textures/controls/button_A.png");
const sf::Texture Textures::button_B = setTexture("resources/textures/controls/button_B.png");
const sf::Texture Textures::button_X = setTexture("resources/textures/controls/button_X.png");
const sf::Texture Textures::button_Y = setTexture("resources/textures/controls/button_Y.png");
const sf::Texture Textures::button_LB = setTexture("resources/textures/controls/button_LB.png");
const sf::Texture Textures::button_RB = setTexture("resources/textures/controls/button_RB.png");
const sf::Texture Textures::button_LS = setTexture("resources/textures/controls/button_LS.png");
const sf::Texture Textures::button_RS = setTexture("resources/textures/controls/button_RS.png");
const sf::Texture Textures::button_LT = setTexture("resources/textures/controls/button_LT.png");
const sf::Texture Textures::button_RT = setTexture("resources/textures/controls/button_RT.png");
const sf::Texture Textures::button_Back = setTexture("resources/textures/controls/button_Back.png");
const sf::Texture Textures::button_Start = setTexture("resources/textures/controls/button_Start.png");
const sf::Texture Textures::button_Left = setTexture("resources/textures/controls/button_Left.png");
const sf::Texture Textures::button_Right = setTexture("resources/textures/controls/button_Right.png");
const sf::Texture Textures::button_Down = setTexture("resources/textures/controls/button_Down.png");
const sf::Texture Textures::button_Up = setTexture("resources/textures/controls/button_Up.png");

BackgroundPalette::BackgroundPalette()
{
	enum TOD { DAY, AFTERNOON, EVENING, NITE, DNITE, MORNING };

	for (int i = 0; i < CORNERS; i++)
	{
		//BOTTOM
		if (i != 1 && i != 2) {
			day[i].r = 0;
			day[i].g = 155;
			day[i].b = 255;

			afternoon[i].r = 20;
			afternoon[i].g = 135;
			afternoon[i].b = 255;

			evening[i].r = 100;
			evening[i].g = 65;
			evening[i].b = 255;

			night[i].r = 70;
			night[i].g = 50;
			night[i].b = 120;

			dark_night[i].r = 10;
			dark_night[i].g = 15;
			dark_night[i].b = 60;

			morning[i].r = 100;
			morning[i].g = 75;
			morning[i].b = 202;

			overcast[i].r = 175;
			overcast[i].g = 175;
			overcast[i].b = 175;
		}
		else //TOP
		{
			day[i].r = 0;
			day[i].g = 60;
			day[i].b = 255;

			afternoon[i].r = 50;
			afternoon[i].g = 125;
			afternoon[i].b = 255;

			evening[i].r = 0;
			evening[i].g = 50;
			evening[i].b = 175;

			night[i].r = 25;
			night[i].g = 25;
			night[i].b = 75;

			dark_night[i].r = 10;
			dark_night[i].g = 10;
			dark_night[i].b = 20;

			morning[i].r = 10;
			morning[i].g = 10;
			morning[i].b = 50;

			overcast[i].r = 215;
			overcast[i].g = 215;
			overcast[i].b = 215;
		}

		black[i].r = 0;
		black[i].g = 0;
		black[i].b = 0;
	}
}

ColorSet & ColorSet::operator=(const ColorSet & clr)
{
	body = clr.body;
	outline = clr.outline;
	return *this;
}

ColorSet::ColorSet(geomelt::Color body, geomelt::Color outline)
{
	this->body = body;
	this->outline = outline;
}

CharacterColorSet & CharacterColorSet::operator=(const CharacterColorSet & clr)
{
	body = clr.body;
	outline = clr.outline;
	reflection = clr.reflection;
	return *this;
}

CharacterColorSet::CharacterColorSet(geomelt::Color body, geomelt::Color reflection, geomelt::Color outline)
{
	this->body = body;
	this->reflection = reflection;
	this->outline = outline;
}

map<CharColorOptions, CharacterColorSet> CharacterPalette::init()
{
	map<CharColorOptions, CharacterColorSet> palette;

	palette[RED] = CharacterColorSet(
			geomelt::Color(255, 75, 75, 255),
			geomelt::Color(255, 125, 125, 255),
			Palette::black
	);

	palette[GREEN] = CharacterColorSet(
		geomelt::Color(75, 255, 75, 255),
		geomelt::Color(150, 255, 150, 255),
		Palette::black
	);

	palette[BLUE] = CharacterColorSet(
		geomelt::Color(150, 150, 255, 255),
		geomelt::Color(180, 180, 255, 255),
		Palette::black
	);

	palette[YELLOW] = CharacterColorSet(
		geomelt::Color(255, 225, 0, 255),
		geomelt::Color(255, 250, 0, 255),
		Palette::black
	);

	palette[PINK] = CharacterColorSet(
		geomelt::Color(255, 150, 150, 255),
		geomelt::Color(255, 180, 180, 255),
		Palette::black
	);

	palette[PURPLE] = CharacterColorSet(
		geomelt::Color(85, 35, 160, 255),
		geomelt::Color(100, 50, 175, 255),
		Palette::black
	);

	palette[WHITE] = CharacterColorSet(
		geomelt::Color(225, 225, 225, 255),
		geomelt::Color(255, 255, 255, 255),
		Palette::black
	);

	palette[BLACK] = CharacterColorSet(
		geomelt::Color(75, 75, 75, 255),
		geomelt::Color(125, 125, 125, 255),
		Palette::black
	);

	return palette;
}

sf::Texture Textures::setTexture(string path)
{
	sf::Texture texture;
	texture.loadFromFile(path);
	return texture;
}
