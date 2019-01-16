#include "menus.h"

MainMenu::MainMenu(Assets assets)
{
	/*
	text.setFont(assets.font);
	text.setString("Geometric Meltdown");
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::White);
	text.setPosition(0.0f, 0.0f);
	*/

	title.set_texture_attributes(assets.textures.title);
	title.body.center.y = HDY / 2;

	play.set_texture_attributes(assets.textures.play);
	play.body.center.y = -HDY / 6;

	options.set_texture_attributes(assets.textures.options);
	options.body.center.y = -HDY / 3;
	
	exit.set_texture_attributes(assets.textures.exit);
	exit.body.center.y = -HDY / 2;

	selected = PLAY;
	selectedIcon.set_texture_attributes(assets.textures.playSelected);
	selectedIcon.body.center.y = play.body.center.y;
}

MainMenu::~MainMenu()
{

}

void MainMenu::handler()
{
	//Fixed Camera
	glOrtho(-HDX, HDX, -HDY, HDY, -1, 1);
	glClear(1);
	title.render();

	play.render();
	options.render();
	exit.render();
	selectedIcon.render();
}

void MainMenu::read_axis(unsigned int joyID, Assets assets)
{
	//Add a disclaimer message which forces players to use dpad to register
	bool modified = false;

	float axis_position1 = sf::Joystick::getAxisPosition(joyID, sf::Joystick::PovY); //DPAD
	float axis_position2 = sf::Joystick::getAxisPosition(joyID, sf::Joystick::Y); //LEFT ANALOG

	//toggle moving condition
	if (axis_position1 == 100 || axis_position2 == -100) //up
	{
		selected--;
		modified = true;
	}
	else if (axis_position1 == -100 || axis_position2 == 100) //down
	{
		selected++;
		modified = true;
	}

	if (modified)
	{
		switch (selected)
		{
		case PLAY:
			selectedIcon.set_texture_attributes(assets.textures.playSelected);
			selectedIcon.body.center.y = play.body.center.y;
			break;
		case OPTIONS:
			selectedIcon.set_texture_attributes(assets.textures.optionsSelected);
			selectedIcon.body.center.y = options.body.center.y;
			break;
		case EXIT:
			selectedIcon.set_texture_attributes(assets.textures.exitSelected);
			selectedIcon.body.center.y = exit.body.center.y;
			break;
		}
	}
	modified = false;
}

void MainMenu::read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<sf::RenderWindow> &window)
{
	switch (event.joystickButton.button)
	{
	case A:
		switch (selected)
		{
		case PLAY:	
			current.menu = CHARSEL;	
			current.menuChange = true;
			break;
		case OPTIONS:

			break;
		case EXIT:
			window->close();
			break;
		}
		break;
	default:
		break;
	}
}

CharacterSelect::CharacterSelect(Assets assets)
{
	//Background Attribute Assignment
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 2.0f * HDX;
	background.body.height = 2.0f * HDY;

	for (int i = 0; i < CORNERS; i++)
	{
		background.color[i].r = assets.palette.lightGrey.r;
		background.color[i].g = assets.palette.lightGrey.g;
		background.color[i].b = assets.palette.lightGrey.b;
	}

	//assign center of each char select box
	float wSpace = -3.0f * HDX / 4.0f;

	for (int i = 0; i < 4; i++)	{
		CharSelBox sb;

		sb.stroke = 30.0f;
		sb.box.center.x = wSpace;
		sb.box.center.y = -HDY / 2.5f;
		sb.box.width = HDX / 3.0f;
		sb.box.height = 3.0f * HDY / 4.0f;
		sb.box.build();
		sb.box.set_color(assets.palette.white);
		sb.outline.center.x = sb.box.center.x;
		sb.outline.center.y = sb.box.center.y;
		sb.outline.width = sb.box.width + sb.stroke;
		sb.outline.height = sb.box.height + sb.stroke;
		sb.outline.build();
		sb.outline.set_color(assets.palette.black);
		sb.start_icon.set_texture_attributes(assets.textures.button_Start);
		sb.start_icon.body.width = 100;
		sb.start_icon.body.height = 100;
		sb.start_icon.body.center.x = sb.box.center.x - (1.5f / 4.0f  * sb.box.width);
		sb.start_icon.body.center.y = sb.box.center.y;
		selectBox.push_back(sb);

		wSpace += HDX / 2.0f;
	}
}

void CharacterSelect::handler(Camera camera, Assets assets, map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin)
{
	//Fixed Camera
	glOrtho(camera.ortho.left, camera.ortho.right, camera.ortho.bottom, camera.ortho.top, -1, 1);

	background.render();

	for (int i = 0; i < 4; i++)	{
		selectBox[i].outline.render();
		selectBox[i].box.render();

		if (selectBox[i].occupied == false)
			selectBox[i].start_icon.render();
	}

	while (it != fin) {
		*it->second = assets.characterPalette.traverse_colors[it->second->myColor];
		it->second->body->center.x = selectBox[it->second->myID].box.center.x;
		it->second->body->center.y = selectBox[it->second->myID].box.center.y;
		it->second->simple_update_menu();
		it->second->render();

		it++;
	}
}

LevelSelect::LevelSelect(Assets assets)
{
	position = 0;

	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 2.0f * HDX;
	background.body.height = 2.0f * HDY;

	for (int i = 0; i < CORNERS; i++) {
		background.color[i].r = assets.palette.lightGrey.r;
		background.color[i].g = assets.palette.lightGrey.g;
		background.color[i].b = assets.palette.lightGrey.b;
	}

	level1.set_texture_attributes(assets.textures.field);
	level1.body.center.y = 0;
	level1.body.center.x = -HDY;
	level1.body.width = 600;
	level1.body.height = 400;

	level2.set_texture_attributes(assets.textures.night);
	level2.body.center.y = 0;
	level2.body.center.x = 0;
	level2.body.width = 600;
	level2.body.height = 400;

	level3.set_texture_attributes(assets.textures.time); 
	level3.body.center.y = 0;
	level3.body.center.x = HDY;
	level3.body.width = 600;
	level3.body.height = 400;

	selector.width = level1.body.width + 16;
	selector.height = level1.body.height + 16;
	selector.center.x = level1.body.center.x;
	selector.center.y = level1.body.center.y;
	selector.color = assets.palette.green;
}

void LevelSelect::handler() 
{
	glOrtho(-HDX, HDX, -HDY, HDY, -1, 1);

	background.render();
	selector.render();
	level1.render();
	level2.render();
	level3.render();
}

void LevelSelect::read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level)
{
	switch (event.joystickButton.button)
	{
	case A:
	{
		current.menu = NONE;
		current.menuChange = true;
		current.render = LEVEL;
		
		switch (position)
		{
		case 0:
			level = unique_ptr<Level>(new Field_Level(assets));
			break;
		case 1:
			level = unique_ptr<Level>(new Night_Level(assets));
			break;
		case 2:
			level = unique_ptr<Level>(new Time_Level(assets));
			break;
		}

		level->reset_level();
	}
		break;
	case B:
	{
		//go back to prev menu
		current.menu = CHARSEL;
		current.menuChange = true;
	}
		break;
	default:
		break;
	}
}

void LevelSelect::read_axis(unsigned int joyID, Assets assets)
{
	float axis_position1 = sf::Joystick::getAxisPosition(joyID, sf::Joystick::PovX); //DPAD
	float axis_position2 = sf::Joystick::getAxisPosition(joyID, sf::Joystick::X); //LEFT ANALOG

	if (axis_position1 == 100 || axis_position2 == 100) 
		position++;
	else if (axis_position1 == -100 || axis_position2 == -100) 
		position--;

	//Prevent underflow
	if (position == UINT_MAX)
		position = 2;

	position = position % 3;

	switch (position)
	{
	case 0:
		selector.center.x = level1.body.center.x;
		selector.center.y = level1.body.center.y;
		break;
	case 1:
		selector.center.x = level2.body.center.x;
		selector.center.y = level2.body.center.y;
		break;
	case 2:
		selector.center.x = level3.body.center.x;
		selector.center.y = level3.body.center.y;
		break;
	}
}