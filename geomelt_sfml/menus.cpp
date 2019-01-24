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
	title.center.y = SCRN_HT / 2;
	title.width *= 2;
	title.height *= 2;
	title.boundary_assignment();

	TexturedQuad play, options, exit;

	play.set_texture_attributes(Assets::textures.play);
	play.center.y = -SCRN_HT / 6;
	play.boundary_assignment();
	
	options.set_texture_attributes(Assets::textures.options);
	options.center.y = -SCRN_HT / 3;
	options.boundary_assignment();

	exit.set_texture_attributes(Assets::textures.exit);
	exit.center.y = -SCRN_HT / 2;
	exit.boundary_assignment();

	navigable.push_back(make_unique<TexturedQuad>(play));
	navigable.push_back(make_unique<TexturedQuad>(options));
	navigable.push_back(make_unique<TexturedQuad>(exit));
	

	/* Add the selected version to the cursor vector */
	vector<unique_ptr<geomelt::Shape>> selected;

	play.set_texture_attributes(Assets::textures.playSelected);
	play.center.y = -SCRN_HT / 6;
	
	options.set_texture_attributes(Assets::textures.optionsSelected);
	options.center.y = -SCRN_HT / 3;

	exit.set_texture_attributes(Assets::textures.exitSelected);
	exit.center.y = -SCRN_HT / 2;
	
	selected.push_back(make_unique<TexturedQuad>(play));
	selected.push_back(make_unique<TexturedQuad>(options));
	selected.push_back(make_unique<TexturedQuad>(exit));
	
	cursor = unique_ptr<Cursor>(new Cursor(selected));
}

Cursor::Cursor(vector<unique_ptr<geomelt::Shape>>& vect)
{
	icons = make_unique<Navigable>(vect);
	selected = icons->begin();
}

void Cursor::updateSelection() {
	if (selected == icons->end())
		selected = icons->begin();
	else if (selected == icons->begin() - 1)
		selected = icons->begin() + 2;
}

void Cursor::render()
{
	selected->get()->render();
}


Navigable::Navigable(vector<unique_ptr<geomelt::Shape>>& vec)
{
	textures.insert(textures.end(), std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()));
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

	for (vector<unique_ptr<geomelt::Shape>>::iterator it = navigable.begin(); it != navigable.end(); ++it)
		it->get()->render();
	
	cursor->render();
	
}

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
		sb.start_icon.width = 100;
		sb.start_icon.height = 100;
		sb.start_icon.center.x = sb.box.center.x - (1.5f / 4.0f  * sb.box.width);
		sb.start_icon.center.y = sb.box.center.y;
		selectBox.push_back(sb);

		wSpace += SCRN_WD / 2.0f;
	}
}

void CharacterSelect::handler(map<unsigned int, unique_ptr<Player>>& players)
{
	//Fixed Camera
	glOrtho(Camera::ortho.left, Camera::ortho.right, Camera::ortho.bottom, Camera::ortho.top, -1, 1);
	glClear(1);

	background.render();

	for (int i = 0; i < CORNERS; i++)	{
		selectBox[i].outline.render();
		selectBox[i].box.render();

		if (selectBox[i].occupied == false)
			selectBox[i].start_icon.render();
	}

	map<unsigned int, unique_ptr<Player>>::iterator it = players.begin();
	
	while (it != players.end()) {
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

	TexturedQuad level1, level2, level3;

	level1.set_texture_attributes(Assets::textures.field);
	level1.center.y = 0;
	level1.center.x = -SCRN_HT;
	level1.width = 640;
	level1.height = 400;

	level2.set_texture_attributes(Assets::textures.night);
	level2.center.y = 0;
	level2.center.x = 0;
	level2.width = 640;
	level2.height = 400;

	level3.set_texture_attributes(Assets::textures.time); 
	level3.center.y = 0;
	level3.center.x = SCRN_HT;
	level3.width = 640;
	level3.height = 400;
	
	navigable.push_back(make_unique<TexturedQuad>(level1));
	navigable.push_back(make_unique<TexturedQuad>(level2));
	navigable.push_back(make_unique<TexturedQuad>(level3));

	/* Add the selected version to the cursor vector */
	vector<unique_ptr<geomelt::Shape>> selected;

	geomelt::Quad selector;
	selector.center.x = level1.center.x;
	selector.center.y = level1.center.y;
	selector.width = 650;
	selector.height = 410;
	selector.color = Assets::palette.green;
	selected.push_back(make_unique<geomelt::Quad>(selector));

	selector.center.x = level2.center.x;
	selector.center.y = level2.center.y;
	selector.color = Assets::palette.green;
	selected.push_back(make_unique<geomelt::Quad>(selector));

	selector.center.x = level3.center.x;
	selector.center.y = level3.center.y;
	selector.color = Assets::palette.green;
	selected.push_back(make_unique<geomelt::Quad>(selector));

	cursor = unique_ptr<Cursor>(new Cursor(selected));
}

void LevelSelect::handler() 
{
	glOrtho(-SCRN_WD, SCRN_WD, -SCRN_HT, SCRN_HT, -1, 1);
	glClear(1);

	background.render();

	cursor->render();

	for (vector<unique_ptr<geomelt::Shape>>::iterator it = navigable.begin(); it != navigable.end(); ++it)
		it->get()->render();
}

void Pause::handler(unique_ptr<Level>& level)
{
	level->gfx_handler();
}