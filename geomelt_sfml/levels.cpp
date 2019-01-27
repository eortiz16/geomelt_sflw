#include "levels.h"

map<unsigned int, unique_ptr<Player>> Level::playerMap;

unique_ptr<Level> Level::make(Lvl lvl)
{
	switch (lvl) {
	case FIELD:
		return unique_ptr<Level>(new Field_Level);
		break;
	case NIGHT:
		return unique_ptr<Level>(new Night_Level);
		break;
	case TIME:
		return unique_ptr<Level>(new Time_Level);
		break;
	default:
		return NULL;
		break;
	}
}

void Level::add_player(unsigned int joyID)
{
	bool is_created_already = false;

	if (playerMap.size() >= 0 && playerMap.size() < 8) {
		map<unsigned int, unique_ptr<Player>>::iterator it;

		for (it = playerMap.begin(); it != playerMap.end(); ++it) {
			if (it->first == joyID) // If map exists
				is_created_already = true; //Don't create
		}

		if (is_created_already == false) {
			playerMap[joyID] = unique_ptr<Player>(new Ball());

			Player *plyr = playerMap[joyID].get();
			*plyr = Assets::characterPalette.traverse_colors[plyr->myColor];
		}
	}
}

void Level::purge_players()
{
	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState == ELIMINATED) { //Erase if eliminated
			it->second.reset();
			playerMap.erase(it);
			break;
		}
	}
}

/* Platform[0] is the maain platform */
void Level::set_platforms()
{
	Platform p0, p1, p2, p3;
	//Platform Horizontal Center
	p0.body.center.x = 0;
	p1.body.center.x = -SCRN_HT * 3.0f / 4.0f;
	p2.body.center.x = 0;
	p3.body.center.x = SCRN_HT * 3.0f / 4.0f;

	//Platform Dimensions
	p0.body.width = 5.0f * SCRN_WD / 6.0f;
	p0.body.height = SCRN_HT / 20.0f;
	p0.body.center.y = SCRN_HT / -3.0f;

	p1.body.width = SCRN_WD / 4.0f;
	p1.body.height = SCRN_HT / 20.0f;
	p1.body.center.y = 100.0f;

	p2.body.width = SCRN_WD / 4.0f;
	p2.body.height = SCRN_HT / 20.0f;
	p2.body.center.y = 100.0f;

	p3.body.width = SCRN_WD / 4.0f;
	p3.body.height = SCRN_HT / 20.0f;
	p3.body.center.y = 100.0f;

	p2.body.center.y = SCRN_HT / 4.0f;

	platform.push_back(p0);
	platform.push_back(p1);
	platform.push_back(p2);
	platform.push_back(p3);

	for (unsigned int i = 0; i < platform.size(); i++) {
		//Floor Boundaries - For Physics
		platform.at(i).body.boundary_assignment();

		//outline centered at body
		platform.at(i).outline.center = platform.at(i).body.center;

		platform.at(i).outline.width = platform.at(i).body.width + THICKNESS;
		platform.at(i).outline.height = platform.at(i).body.height + THICKNESS;

		//Assign Color to Floor
		platform.at(i).body.color = Assets::platformPalette.grass.body;
		platform.at(i).outline.color = Assets::platformPalette.grass.outline;

		//Assign Floor Outline Attributes
		platform.at(i).body.boundary_assignment();
	}
}

void Level::reset_level()
{
	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		it->second->body->height = 100;
		it->second->body->width = 100;
		it->second->body->radius = 50;
		it->second->body->boundary_assignment();
		it->second->toggle.on_ground = true;
		it->second->update_reflection_x();
		it->second->simple_update();

		if (it->second->myID % 2 == 0) 
			it->second->body->center = geomelt::Vec(500.0f, 500.0f, 0);
		else
			it->second->body->center = geomelt::Vec(-500.0f, 500.0f, 0);
		
	}
}          

Level::Level()
{
	blackVoid.set_color(Assets::backgroundPalette.black);
	blackVoid.body.center.x = 0;
	blackVoid.body.center.y = 0;
	blackVoid.body.width = 10.0f * SCRN_WD;
	blackVoid.body.height = 10.0f * SCRN_HT;
}

Field_Level::Field_Level() : Level()
{
	srand((unsigned int)time(NULL));

	//Background Color Assignment
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * SCRN_WD;
	background.body.height = 5.0f * SCRN_HT;
	background.set_color(Assets::backgroundPalette.day);

	//Sun Attributes
	sun.radius = (GLfloat)SCRN_HT;
	sun.color = Assets::palette.sun;
	sun.center.x = (GLfloat)SCRN_WD;
	sun.center.y = (GLfloat)SCRN_HT;

	//Wind
	(rand() % 2 == 0) ? windDirection = RIGHT : windDirection = LEFT;

	for (int i = 0; i < MAX_CLOUDS; ++i)
		clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));

	// Initially only - Assign random X coordinate
	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) {
		it->get()->body[1].get()->center.x = (float)(rand() % (4 * SCRN_WD) - 2 * SCRN_WD);
		it->get()->body[0].get()->center.x = it->get()->body[1].get()->center.x - it->get()->body[1].get()->radius;
		it->get()->body[2].get()->center.x = it->get()->body[1].get()->center.x + it->get()->body[1].get()->radius;
	}

	set_platforms();
}

void Field_Level::update_clouds()
{
	GLfloat arg1, arg2;
	int offset = 0;

	for (vector<unique_ptr<Cloud>>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
		// Parameters to determine when a cloud is off screen
		arg1 = it->get()->body[1]->center.x + (it->get()->body[1]->radius * 3); // Left
		arg2 = it->get()->body[1]->center.x - (it->get()->body[1]->radius * 3); // Right
		offset = it - clouds.begin();

		//Reset if Last Cloud Offscreen
		if (arg1 < -SCRN_WD && windDirection == LEFT) {
			clouds.erase(clouds.begin() + offset);
			clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));
		}
		else if (arg2 > SCRN_WD && windDirection == RIGHT) {
			clouds.erase(clouds.begin() + offset);
			clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));
		}

		it->get()->update(windDirection);
	}
}

void Field_Level::purge_clouds()
{
	for (unsigned int i = 0; i < clouds.size(); ++i) {
		if (clouds[i].get()->offScreen) {
			clouds.erase(clouds.begin() + i);
			clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));
		}
	}
}

void Field_Level::gfx_handler()
{
	//Dynamic Camera
	glOrtho(Camera::edges.left, Camera::edges.right, Camera::edges.bottom, Camera::edges.top, -1, 1);

	render();

	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {		
		if (it->second->stats.lifeState == ALIVE)
			it->second->render();
	}
}

void Field_Level::phys_handler()
{
	update_clouds();
	purge_clouds();

	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState != ELIMINATED)
			it->second->update_position(platform);

		it->second->death_handler();
	}

	Camera::set_center(playerMap);
	Camera::set_edges();
}

void Field_Level::render()
{
	blackVoid.render();
	background.render();
	sun.render();

	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) 
		it->get()->render();
	
	for (vector<Platform>::iterator it = platform.begin(); it != platform.end(); ++it) {
		it->outline.render();
		it->body.render();
	}
}

Night_Level::Night_Level() : Level()
{
	//Background Attributes
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * SCRN_WD;
	background.body.height = 5.0f * SCRN_HT;

	//Color assignment
	background.set_color(Assets::backgroundPalette.night);

	//Moon Attributes
	moon.color = Assets::palette.moon;
	moon.center.x = SCRN_WD / 2.0f;
	moon.center.y = SCRN_HT / 2.0f;
	moon.radius = SCRN_HT / 2.0f;

	set_platforms();
}

void Night_Level::gfx_handler()
{
	//Dynamic Camera
	glOrtho(Camera::edges.left, Camera::edges.right, Camera::edges.bottom, Camera::edges.top, -1, 1);

	render();

	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState == ALIVE)
			it->second->render();
	}
}

void Night_Level::phys_handler()
{
	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState != ELIMINATED)
			it->second->update_position(platform);

		it->second->death_handler();
	}

	Camera::set_center(playerMap);
	Camera::set_edges();
}

void Night_Level::render()
{
	blackVoid.render();
	background.render();
	stars.render();
	moon.render();

	for (vector<Platform>::iterator it = platform.begin(); it != platform.end(); ++it) {
		it->outline.render();
		it->body.render();
	}
}

Time_Level::Time_Level() : Level()
{
	srand((unsigned int)time(NULL));

	//Background Attribute Assignment
	timeOfDay = EVENING;
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * SCRN_WD;
	background.body.height = 5.0f * SCRN_HT;

	bg_pal = Assets::backgroundPalette;

	transition = false;

	//Initialize Background
	background.set_color(Assets::backgroundPalette.evening);

	//Sun Attributes
	sun.radius = (GLfloat)SCRN_HT;
	sun.color = Assets::palette.sun;
	sun.center.x = (GLfloat)SCRN_WD;
	sun.center.y = (GLfloat)SCRN_HT;

	//Moon Attributes
	moon.color = Assets::palette.moon;
	moon.center.x = SCRN_WD / 2.0f;
	moon.center.y = SCRN_HT / 2.0f;
	moon.radius = SCRN_HT / 2.0f;

	//Wind
	(rand() % 2 == 0) ? windDirection = RIGHT : windDirection = LEFT;

	for (int i = 0; i < MAX_CLOUDS; ++i)
		clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));

	// Initially only Assign random X coordinate
	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) {
		int randX = rand() % (4 * SCRN_WD) - 2 * SCRN_WD; // get random position for cloud
		it->get()->body[1].get()->center.x = (float)randX;
		it->get()->body[0].get()->center.x = randX - it->get()->body[1].get()->radius;
		it->get()->body[2].get()->center.x = randX + it->get()->body[1].get()->radius;
	}

	set_platforms();
}

void Time_Level::update_clouds()
{
	GLfloat arg1, arg2;
	int offset = 0;

	for (vector<unique_ptr<Cloud>>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
		// Parameters to determine when a cloud is off screen
		arg1 = it->get()->body[1]->center.x + (it->get()->body[1]->radius * 3);
		arg2 = it->get()->body[1]->center.x - (it->get()->body[1]->radius * 3);
		offset = it - clouds.begin();

		//Reset if Last Cloud Offscreen
		if (arg1 < -SCRN_WD && windDirection == LEFT) {
			clouds.erase(clouds.begin() + offset);
			clouds.push_back(make_unique<Cloud>(Cloud::make_cloud(windDirection)));
		}
		else if (arg2 > SCRN_WD && windDirection == RIGHT) {
			clouds.erase(clouds.begin() + offset);
			clouds.push_back(make_unique<Cloud>(Cloud::make_cloud(windDirection)));
		}
		it->get()->update(windDirection);
	}
}

void Time_Level::purge_clouds()
{
	for (unsigned int i = 0; i < clouds.size(); ++i) {
		if (clouds[i].get()->offScreen) {
			clouds.erase(clouds.begin() + i);
			clouds.push_back(make_unique<Cloud>(Cloud::make_cloud(windDirection)));
		}
	}
}

/* Great Candidate for State Design Pattern */
void Time_Level::transition_handler()
{
	if (transition == true)
		switch (timeOfDay)
		{
			case DAY:
				transition_to(Assets::backgroundPalette.afternoon);
				break;
			case AFTERNOON:
				transition_to(Assets::backgroundPalette.evening);
				break;
			case EVENING:
				transition_to(Assets::backgroundPalette.night);
				break;
			case NITE:
				transition_to(Assets::backgroundPalette.dark_night);
				break;
			case DNITE:
				transition_to(Assets::backgroundPalette.morning);
				break;
			case MORNING:
				transition_to(Assets::backgroundPalette.day);
				break;
		}
}

void Time_Level::gfx_handler()
{
	//Dynamic Camera
	glOrtho(Camera::edges.left, Camera::edges.right, Camera::edges.bottom, Camera::edges.top, -1, 1);

	render();

	map<unsigned int, unique_ptr<Player>>::iterator it;
	
	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState == ALIVE)
			it->second->render();
	}
}

void Time_Level::phys_handler()
{
	transition_handler();
	
	update_clouds();
	purge_clouds();
	
	map<unsigned int, unique_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState != ELIMINATED)
			it->second->update_position(platform);
		it->second->death_handler();
	}

	Camera::set_center(playerMap);
	Camera::set_edges();
}

void Time_Level::render()
{
	blackVoid.render();
	background.render();

	switch (timeOfDay)
	{
	case DAY:
	case AFTERNOON:
		sun.render();
		break;
	default:
		stars.render();
		moon.render();
		break;
	}
	
	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it)
		it->get()->render();

	for (vector<Platform>::iterator it = platform.begin(); it != platform.end(); ++it) {
		it->outline.render();
		it->body.render();
	}
}

//changes color of background by factor of 1 each frame
void Time_Level::transition_to(geomelt::Color *clr)
{
	//Transition from bg to clr
	geomelt::Color *bg;

	//Adjust Color of Corners
	for (int i = 0; i < 4; i++)	{
		bg = &background.color[i];

		//Update RED
		if (bg->r < clr[i].r)
			bg->r += TRANSITION_RATE_TOD;
		else if (bg->r > clr[i].r)
			bg->r -= TRANSITION_RATE_TOD;

		//Update GREEN
		if (bg->g < clr[i].g)
			bg->g += TRANSITION_RATE_TOD;
		else if (bg->g > clr[i].g)
			bg->g -= TRANSITION_RATE_TOD;

		//Update BLUE
		if (bg->b < clr[i].b)
			bg->b += TRANSITION_RATE_TOD;
		else if (bg->b > clr[i].b)
			bg->b -= TRANSITION_RATE_TOD;

		//If all corners are done updating
		if (bg->r == clr[i].r && bg->g == clr[i].g && bg->b == clr[i].b)
			background.transition_done[i] = true;
	}

	if (background.transition_done[0] == true && background.transition_done[1] == true
		&& background.transition_done[2] == true && background.transition_done[3] == true) {
		//Transition is Done, Reset Attributes
		for (int i = 0; i < CORNERS; i++)
			background.transition_done[i] = false;

		transition = false;
		timeOfDay++;
	}
}