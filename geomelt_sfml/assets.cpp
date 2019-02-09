#include "assets.h"

Textures Assets::textures;
Palette Assets::palette;
BackgroundPalette Assets::backgroundPalette;
CharacterPalette Assets::characterPalette;
const Color Palette::sun = Color(255, 255, 100, 255);
const Color Palette::moon = Color(225, 225, 215, 255);
const Color Palette::platform = Color(99, 160, 0, 255);
const Color Palette::black = Color(0, 0, 0, 255);
const Color Palette::grey = Color(150, 150, 150, 255);
const Color Palette::darkGrey = Color(25, 25, 25, 255);
const Color Palette::lightGrey = Color(210, 210, 210, 255);
const Color Palette::white = Color(255, 255, 255, 255);
const Color Palette::red = Color(255, 0, 0, 255);
const Color Palette::darkRed = Color(150, 0, 0, 255);
const Color Palette::green = Color(0, 255, 0, 255);
const Color Palette::darkGreen = Color(0, 150, 0, 255);
const Color Palette::blue = Color(0, 0, 255, 255);
const Color Palette::darkBlue = Color(0, 0, 150, 255);
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

	for (int i = 0; i < CORNERS; ++i) {
		day.push_back(Color());
		afternoon.push_back(Color());
		evening.push_back(Color());
		night.push_back(Color());
		dark_night.push_back(Color());
		morning.push_back(Color());
		overcast.push_back(Color());
		overcast.push_back(Color(0, 0, 0, 255));
	}

	for (int i = 0; i < CORNERS; i++) {
		//BOTTOM
		if (i != 1 && i != 2) {
			day[i] = Color(0, 155, 255, 255);
			afternoon[i] = Color(20, 135, 255, 255);
			evening[i] = Color(100, 65, 255, 255);
			night[i] = Color(70, 50, 120, 255);
			dark_night[i] = Color(10, 15, 60, 255);
			morning[i] = Color(100, 75, 202, 255);
			overcast[i] = Color(175, 175, 175, 255);
		} else { //TOP
			day[i] = Color(0, 60, 255, 255);
			afternoon[i] = Color(50, 125, 255, 255);
			evening[i] = Color(0, 50, 175, 255);
			night[i] = Color(25, 25, 75, 255);
			dark_night[i] = Color(10, 10, 20, 255);
			morning[i] = Color(10, 10, 50, 255);
			overcast[i] = Color(215, 215, 215, 255);
		}
	}
}

ColorSet & ColorSet::operator=(const ColorSet & clr)
{
	body = clr.body;
	outline = clr.outline;
	return *this;
}

ColorSet::ColorSet(Color body, Color outline)
{
	this->body = body;
	this->outline = outline;
}

CharacterColorSet & CharacterColorSet::operator=(const CharacterColorSet & clr)
{
	this->body = clr.body;
	this->outline = clr.outline;
	this->reflection = clr.reflection;
	return *this;
}

CharacterColorSet::CharacterColorSet(Color body, Color reflection, Color outline)
{
	this->body = body;
	this->reflection = reflection;
	this->outline = outline;
}

map<CharColorOptions, CharacterColorSet> CharacterPalette::init()
{
	map<CharColorOptions, CharacterColorSet> palette;

	palette[RED] = CharacterColorSet(
			Color(255, 75, 75, 255),
			Color(255, 125, 125, 255),
			Palette::black
	);

	palette[GREEN] = CharacterColorSet(
		Color(75, 255, 75, 255),
		Color(150, 255, 150, 255),
		Palette::black
	);

	palette[BLUE] = CharacterColorSet(
		Color(150, 150, 255, 255),
		Color(180, 180, 255, 255),
		Palette::black
	);

	palette[YELLOW] = CharacterColorSet(
		Color(255, 225, 0, 255),
		Color(255, 250, 0, 255),
		Palette::black
	);

	palette[PINK] = CharacterColorSet(
		Color(255, 150, 150, 255),
		Color(255, 180, 180, 255),
		Palette::black
	);

	palette[PURPLE] = CharacterColorSet(
		Color(85, 35, 160, 255),
		Color(100, 50, 175, 255),
		Palette::black
	);

	palette[WHITE] = CharacterColorSet(
		Color(225, 225, 225, 255),
		Color(255, 255, 255, 255),
		Palette::black
	);

	palette[BLACK] = CharacterColorSet(
		Color(75, 75, 75, 255),
		Color(125, 125, 125, 255),
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
