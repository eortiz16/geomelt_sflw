#include "menus.h"

MainMenu::MainMenu()
{
	/*
	text.setFont(assets.font);
	text.setString("Geometric Meltdown");
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::White);
	text.setPosition(0.0f, 0.0f);
	*/
	srand((unsigned int)time(NULL));

	title.set_texture_attributes(Assets::textures.title);
	title.body.center.y = SCRN_HT / 2;
	title.body.width *= 2;
	title.body.height *= 2;
	title.body.boundary_assignment();

	play.set_texture_attributes(Assets::textures.play);
	play.body.center.y = -SCRN_HT / 6;
	play.body.boundary_assignment();

	options.set_texture_attributes(Assets::textures.options);
	options.body.center.y = -SCRN_HT / 3;
	options.body.boundary_assignment();
	
	exit.set_texture_attributes(Assets::textures.exit);
	exit.body.center.y = -SCRN_HT / 2;
	exit.body.boundary_assignment();

	selected = PLAY;
	selectedIcon.set_texture_attributes(Assets::textures.playSelected);
	selectedIcon.body.center.y = play.body.center.y;
}

void MainMenu::handler(unique_ptr<Level>& level)
{
	//Fixed Camera
	glOrtho(-SCRN_WD, SCRN_WD, -SCRN_HT, SCRN_HT, -1, 1);
	glClear(1);

	//draw level
	level->phys_handler();
	level->render();

	title.render();
	play.render();
	options.render();
	exit.render();
	selectedIcon.render();
}

void MainMenu::update_selected()
{
	switch (selected)
	{
	case PLAY:
		selectedIcon.set_texture_attributes(Assets::textures.playSelected);
		selectedIcon.body.center.y = play.body.center.y;
		break;
	case OPTIONS:
		selectedIcon.set_texture_attributes(Assets::textures.optionsSelected);
		selectedIcon.body.center.y = options.body.center.y;
		break;
	case EXIT:
		selectedIcon.set_texture_attributes(Assets::textures.exitSelected);
		selectedIcon.body.center.y = exit.body.center.y;
		break;
	}
}

/*
void MainMenu::read_axis(unsigned int joyID)
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
			selectedIcon.set_texture_attributes(Assets::textures.playSelected);
			selectedIcon.body.center.y = play.body.center.y;
			break;
		case OPTIONS:
			selectedIcon.set_texture_attributes(Assets::textures.optionsSelected);
			selectedIcon.body.center.y = options.body.center.y;
			break;
		case EXIT:
			selectedIcon.set_texture_attributes(Assets::textures.exitSelected);
			selectedIcon.body.center.y = exit.body.center.y;
			break;
		}
	}
	modified = false;
}

int MainMenu::read_buttons(sf::Event event, CurrentGameState &current)
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
			// signal quit
			return -1;
			break;
		}
		break;
	default:
		break;
	}
	return 0;
}
*/

CharacterSelect::CharacterSelect()
{
	//Background Attribute Assignment
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 2.0f * SCRN_WD;
	background.body.height = 2.0f * SCRN_HT;

	for (int i = 0; i < CORNERS; i++)
	{
		background.color[i].r = Assets::palette.lightGrey.r;
		background.color[i].g = Assets::palette.lightGrey.g;
		background.color[i].b = Assets::palette.lightGrey.b;
	}

	//assign center of each char select box
	float wSpace = -3.0f * SCRN_WD / 4.0f;

	for (int i = 0; i < 4; i++)	{
		CharSelBox sb;

		sb.stroke = 30.0f;
		sb.box.center.x = wSpace;
		sb.box.center.y = -SCRN_HT / 2.5f;
		sb.box.width = SCRN_WD / 3.0f;
		sb.box.height = 3.0f * SCRN_HT / 4.0f;
		sb.box.build();
		sb.box.set_color(Assets::palette.white);
		sb.outline.center.x = sb.box.center.x;
		sb.outline.center.y = sb.box.center.y;
		sb.outline.width = sb.box.width + sb.stroke;
		sb.outline.height = sb.box.height + sb.stroke;
		sb.outline.build();
		sb.outline.set_color(Assets::palette.black);
		sb.start_icon.set_texture_attributes(Assets::textures.button_Start);
		sb.start_icon.body.width = 100;
		sb.start_icon.body.height = 100;
		sb.start_icon.body.center.x = sb.box.center.x - (1.5f / 4.0f  * sb.box.width);
		sb.start_icon.body.center.y = sb.box.center.y;
		selectBox.push_back(sb);

		wSpace += SCRN_WD / 2.0f;
	}
}

void CharacterSelect::handler(map<unsigned int, unique_ptr<Player>>& players)
{
	//Fixed Camera
	glOrtho(Camera::ortho.left, Camera::ortho.right, Camera::ortho.bottom, Camera::ortho.top, -1, 1);

	background.render();

	for (int i = 0; i < CORNERS; i++)	{
		selectBox[i].outline.render();
		selectBox[i].box.render();

		if (selectBox[i].occupied == false)
			selectBox[i].start_icon.render();
	}

	map<unsigned int, unique_ptr<Player>>::iterator it = players.begin();
	map<unsigned int, unique_ptr<Player>>::iterator fin = players.end();
	
	while (it != fin) {
		*it->second = Assets::characterPalette.traverse_colors[it->second->myColor];
		it->second->body->center.x = selectBox[it->second->myID].box.center.x;
		it->second->body->center.y = selectBox[it->second->myID].box.center.y;
		it->second->simple_update_menu();
		it->second->render();

		it++;
	}
}

LevelSelect::LevelSelect()
{
	position = 0;

	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 2.0f * SCRN_WD;
	background.body.height = 2.0f * SCRN_HT;

	for (int i = 0; i < CORNERS; i++) {
		background.color[i].r = Assets::palette.lightGrey.r;
		background.color[i].g = Assets::palette.lightGrey.g;
		background.color[i].b = Assets::palette.lightGrey.b;
	}

	level1.set_texture_attributes(Assets::textures.field);
	level1.body.center.y = 0;
	level1.body.center.x = -SCRN_HT;
	level1.body.width = 640;
	level1.body.height = 400;

	level2.set_texture_attributes(Assets::textures.night);
	level2.body.center.y = 0;
	level2.body.center.x = 0;
	level2.body.width = 640;
	level2.body.height = 400;

	level3.set_texture_attributes(Assets::textures.time); 
	level3.body.center.y = 0;
	level3.body.center.x = SCRN_HT;
	level3.body.width = 640;
	level3.body.height = 400;

	selector.width = level1.body.width + 16;
	selector.height = level1.body.height + 16;
	selector.center.x = level1.body.center.x;
	selector.center.y = level1.body.center.y;
	selector.color = Assets::palette.green;
}

void LevelSelect::handler() 
{
	glOrtho(-SCRN_WD, SCRN_WD, -SCRN_HT, SCRN_HT, -1, 1);

	background.render();
	selector.render();
	level1.render();
	level2.render();
	level3.render();
}

/*
void LevelSelect::read_buttons(sf::Event event, CurrentGameState &current, unique_ptr<Level> &level)
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
			level = unique_ptr<Level>(new Field_Level());
			break;
		case 1:
			level = unique_ptr<Level>(new Night_Level());
			break;
		case 2:
			level = unique_ptr<Level>(new Time_Level());
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

void LevelSelect::read_axis(unsigned int joyID)
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
*/