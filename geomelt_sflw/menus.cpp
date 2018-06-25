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
	cout << "initial = " << selected << endl;
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

	float axis_position1 = Joystick::getAxisPosition(joyID, Joystick::PovY); //DPAD
	float axis_position2 = Joystick::getAxisPosition(joyID, Joystick::Y); //LEFT ANALOG

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
		//cout << selected << "#\n";
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

void MainMenu::read_buttons(unsigned int button, Assets assets, CurrentMenu *currentMenu, bool *menuChange)
{
	switch (button)
	{
	case A:
		switch (selected)
		{
		case PLAY:	
			*currentMenu = CHARSEL;	
			*menuChange = true;
			break;
		case OPTIONS:

			break;
		case EXIT:

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

	for (int i = 0; i < 4; i++)
	{
		selectBox[i].stroke = 30.0f;

		selectBox[i].box.center.x = wSpace;
		selectBox[i].box.center.y = -HDY / 2.5f;
		selectBox[i].box.width = HDX / 3.0f;
		selectBox[i].box.height = 3.0f * HDY / 4.0f;
		selectBox[i].box.build();
		selectBox[i].box.set_color(assets.palette.white);

		selectBox[i].outline.center.x = selectBox[i].box.center.x;
		selectBox[i].outline.center.y = selectBox[i].box.center.y;
		selectBox[i].outline.width = selectBox[i].box.width + selectBox[i].stroke;
		selectBox[i].outline.height = selectBox[i].box.height + selectBox[i].stroke;
		selectBox[i].outline.build();
		selectBox[i].outline.set_color(assets.palette.black);

		wSpace += HDX / 2.0f;
	}
}

void CharacterSelect::handler(Camera camera, Assets assets, map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin)
{
	//Fixed Camera
	glOrtho(camera.ortho.left, camera.ortho.right, camera.ortho.bottom, camera.ortho.top, -1, 1);

	background.render();

	for (int i = 0; i < 4; i++)
	{
		selectBox[i].outline.render();
		selectBox[i].box.render();
	}

	while (it != fin)
	{
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

}

void LevelSelect::handler() 
{

}

void LevelSelect::read_buttons(unsigned int button, Render_State *render, CurrentMenu *currentMenu, bool *menuChange, unique_ptr<Level> &level, Assets assets)
{
	switch (button)
	{
	case A:
	{
		*currentMenu = NONE;
		*menuChange = true;
		*render = LEVEL;
		level = unique_ptr<Level>(new Field_Level(assets));
		level->reset(); //Change name of function
	}
		break;
	default:
		break;
	}
}
