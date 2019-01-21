#include "assets.h"

sf::Font Assets::font;
sf::Texture Assets::backgroundTexture;
sf::Texture Assets::texture;
Textures Assets::textures;
sf::Sprite Assets::background;
Palette Assets::palette;
Palette_BG Assets::backgroundPalette;
Palette_CHAR Assets::characterPalette;
Palette_PLAT Assets::platformPalette = Palette_PLAT(palette);

Assets::Assets()
{
	if (!font.loadFromFile("resources/sansation.ttf"))
		cout << "font failed to load\n";

	backgroundTexture.setSrgb(false); //srgb false
	backgroundTexture.loadFromFile("resources/background.jpg");
	texture.loadFromFile("resources/texture.jpg");

	sf::Sprite temp(backgroundTexture);
	background = temp;
}

Palette::Palette()
{
	sun.r = 255;
	sun.g = 255;
	sun.b = 100;
	sun.alpha = 255;

	moon.r = 225;
	moon.g = 225;
	moon.b = 215;
	moon.alpha = 255;

	platform.r = 99;
	platform.g = 160;
	platform.b = 0;
	platform.alpha = 255;

	black.r = 0;
	black.g = 0;
	black.b = 0;
	black.alpha = 255;

	grey.r = 150;
	grey.g = 150;
	grey.b = 150;
	grey.alpha = 255;

	darkGrey.r = 25;
	darkGrey.g = 25;
	darkGrey.b = 25;
	darkGrey.alpha = 255;

	lightGrey.r = 210;
	lightGrey.g = 210;
	lightGrey.b = 210;
	lightGrey.alpha = 255;

	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.alpha = 255;

	red.r = 255;
	red.g = 0;
	red.b = 0;
	red.alpha = 255;

	darkRed.r = 150;
	darkRed.g = 0;
	darkRed.b = 0;
	darkRed.alpha = 255;

	green.r = 0;
	green.g = 255;
	green.b = 0;
	green.alpha = 255;

	darkGreen.r = 0;
	darkGreen.g = 150;
	darkGreen.b = 0;
	darkGreen.alpha = 255;

	blue.r = 0;
	blue.g = 0;
	blue.b = 255;
	blue.alpha = 255;

	darkBlue.r = 0;
	darkBlue.g = 0;
	darkBlue.b = 150;
	darkBlue.alpha = 255;
}

Palette_PLAT::Palette_PLAT(Palette pal)
{
	grass.body = pal.platform;
	grass.outline = pal.black;
}

Palette_CHAR::Palette_CHAR()
{
	red.body.r = 255;
	red.body.g = 75;
	red.body.b = 75;
	red.body.alpha = 255;

	red.reflection.r = 255;
	red.reflection.g = 125;
	red.reflection.b = 125;
	red.reflection.alpha = 255;

	red.outline.r = 0;
	red.outline.g = 0;
	red.outline.b = 0;
	red.outline.alpha = 255;

	traverse_colors[RED] = red;

	green.body.r = 75;
	green.body.g = 255;
	green.body.b = 75;
	green.body.alpha = 255;

	green.reflection.r = 150;
	green.reflection.g = 255;
	green.reflection.b = 150;
	green.reflection.alpha = 255;

	green.outline.r = 0;
	green.outline.g = 0;
	green.outline.b = 0;
	green.outline.alpha = 255;

	traverse_colors[GREEN] = green;

	blue.body.r = 150;
	blue.body.g = 150;
	blue.body.b = 255;
	blue.body.alpha = 255;

	blue.reflection.r = 180;
	blue.reflection.g = 180;
	blue.reflection.b = 255;
	blue.reflection.alpha = 255;

	blue.outline.r = 0;
	blue.outline.g = 0;
	blue.outline.b = 0;
	blue.outline.alpha = 255;

	traverse_colors[BLUE] = blue;

	yellow.body.r = 255;
	yellow.body.g = 225;
	yellow.body.b = 0;
	yellow.body.alpha = 255;

	yellow.reflection.r = 255;
	yellow.reflection.g = 250;
	yellow.reflection.b = 0;
	yellow.reflection.alpha = 255;
	
	yellow.outline.r = 0;
	yellow.outline.g = 0;
	yellow.outline.b = 0;
	yellow.outline.alpha = 255;

	traverse_colors[YELLOW] = yellow;

	pink.body.r = 255;
	pink.body.g = 150;
	pink.body.b = 150;
	pink.body.alpha = 255;

	pink.reflection.r = 255;
	pink.reflection.g = 180;
	pink.reflection.b = 180;
	pink.reflection.alpha = 255;

	pink.outline.r = 0;
	pink.outline.g = 0;
	pink.outline.b = 0;
	pink.outline.alpha = 255;

	traverse_colors[PINK] = pink;

	purple.body.r = 85;
	purple.body.g = 35;
	purple.body.b = 160;
	purple.body.alpha = 255;

	purple.reflection.r = 100;
	purple.reflection.g = 50;
	purple.reflection.b = 175;
	purple.reflection.alpha = 255;

	purple.outline.r = 0;
	purple.outline.g = 0;
	purple.outline.b = 0;
	purple.outline.alpha = 255;

	traverse_colors[PURPLE] = purple;

	white.body.r = 225;
	white.body.g = 225;
	white.body.b = 225;
	white.body.alpha = 255;

	white.reflection.r = 255;
	white.reflection.g = 255;
	white.reflection.b = 255;
	white.reflection.alpha = 255;
	
	white.outline.r = 0;
	white.outline.g = 0;
	white.outline.b = 0;
	white.outline.alpha = 255;

	traverse_colors[WHITE] = white;

	black.body.r = 75;
	black.body.g = 75;
	black.body.b = 75;
	black.body.alpha = 255;

	black.reflection.r = 125;
	black.reflection.g = 125;
	black.reflection.b = 125;
	black.reflection.alpha = 255;

	black.outline.r = 0;
	black.outline.g = 0;
	black.outline.b = 0;
	black.outline.alpha = 255;

	traverse_colors[BLACK] = black;
}


Palette_BG::Palette_BG()
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

Textures::Textures()
{
	field.loadFromFile("resources/textures/field.png");
	night.loadFromFile("resources/textures/night.png");
	time.loadFromFile("resources/textures/time.png");
	title.loadFromFile("resources/textures/title.png");

	play.loadFromFile("resources/textures/play_u.png");
	playSelected.loadFromFile("resources/textures/play_s.png");
	options.loadFromFile("resources/textures/options_u.png");
	optionsSelected.loadFromFile("resources/textures/options_s.png");
	exit.loadFromFile("resources/textures/exit_u.png");
	exitSelected.loadFromFile("resources/textures/exit_s.png");

	button_A.loadFromFile("resources/textures/controls/button_A.png");
	button_B.loadFromFile("resources/textures/controls/button_B.png");
	button_X.loadFromFile("resources/textures/controls/button_X.png");
	button_Y.loadFromFile("resources/textures/controls/button_Y.png");
	button_LB.loadFromFile("resources/textures/controls/button_LB.png");
	button_RB.loadFromFile("resources/textures/controls/button_RB.png");
	button_LS.loadFromFile("resources/textures/controls/button_LS.png");
	button_RS.loadFromFile("resources/textures/controls/button_RS.png");
	button_LT.loadFromFile("resources/textures/controls/button_LT.png");
	button_RT.loadFromFile("resources/textures/controls/button_RT.png");
	button_Back.loadFromFile("resources/textures/controls/button_Back.png");
	button_Start.loadFromFile("resources/textures/controls/button_Start.png");
	button_Left.loadFromFile("resources/textures/controls/button_Left.png");
	button_Right.loadFromFile("resources/textures/controls/button_Right.png");
	button_Down.loadFromFile("resources/textures/controls/button_Down.png");
	button_Up.loadFromFile("resources/textures/controls/button_Up.png");
}