#include "levels.h"

map<unsigned int, shared_ptr<Player>> Level::playerMap;

void Level::add_player(unsigned int joyID)
{
	bool is_created_already = false;

	if (playerMap.size() >= 0 && playerMap.size() < 8) {
		map<unsigned int, shared_ptr<Player>>::iterator it;

		for (it = playerMap.begin(); it != playerMap.end(); ++it) {
			if (it->first == joyID) // If map exists
				is_created_already = true; //Don't create
		}

		if (is_created_already == false) {
			playerMap[joyID] = shared_ptr<Player>(new Ball());

			Player *plyr = playerMap[joyID].get();
			*plyr = Assets::characterPalette.traverse_colors[plyr->myColor];
		}
	}
}

void Level::purge_players()
{
	map<unsigned int, shared_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState == ELIMINATED) { //Erase if eliminated
			it->second.reset();
			playerMap.erase(it);
			break;
		}
	}
}

void Level::reset_level()
{
	map<unsigned int, shared_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		it->second->body->height = 100;
		it->second->body->width = 100;
		it->second->body->radius = 50;
		it->second->body->boundary_assignment();
		it->second->toggle.on_ground = true;
		it->second->update_reflection_x();
		it->second->simple_update();

		if (it->second->myID % 2 == 0) {
			it->second->body->center.x = 500.0f;
			it->second->body->center.y = 500.0f;
		} else {
			it->second->body->center.x = -500.0f;
			it->second->body->center.y = 500.0f;
		}
	}
}

void GradientBG::render()
{
	//Top LEFT
	glBegin(GL_QUADS);
		//bottom right
		glColor4ub(255, 255, 255, 0);
		glVertex2f(TLBG.body.center.x + TLBG.body.width / 2, TLBG.body.center.y - TLBG.body.height / 2);

		//bottom left
		glColor4ub(0, 155, 255, 200);
		glVertex2f(TLBG.body.center.x - TLBG.body.width / 2, TLBG.body.center.y - TLBG.body.height / 2);

		//top left
		glColor4ub(0,155,255, 200);
		glVertex2f(TLBG.body.center.x - TLBG.body.width / 2, TLBG.body.center.y + TLBG.body.height / 2);

		//top right
		glColor4ub(255,255,255, 0);
		glVertex2f(TLBG.body.center.x + TLBG.body.width / 2, TLBG.body.center.y + TLBG.body.height / 2);	
	glEnd();

	//TOP RIGHT
	glBegin(GL_QUADS);
		//bottom left
		glColor4ub(255, 255, 255, 0);
		glVertex2f(TRBG.body.center.x - TRBG.body.width / 2, TRBG.body.center.y - TRBG.body.height / 2);

		//bottom right
		glColor4ub(255, 155, 0, 200);
		glVertex2f(TRBG.body.center.x + TRBG.body.width / 2, TRBG.body.center.y - TRBG.body.height / 2);

		//top right
		glColor4ub(255, 155, 0, 200);
		glVertex2f(TRBG.body.center.x + TRBG.body.width / 2, TRBG.body.center.y + TRBG.body.height / 2);

		//top left
		glColor4ub(255, 255, 255, 0);
		glVertex2f(TRBG.body.center.x - TRBG.body.width / 2, TRBG.body.center.y + TRBG.body.height / 2);
	glEnd();
}                

Level::Level()
{
	blackVoid.set_color(Assets::backgroundPalette.black);
	blackVoid.body.center.x = 0;
	blackVoid.body.center.y = 0;
	blackVoid.body.width = 10.0f * HDX;
	blackVoid.body.height = 10.0f * HDY;
}

Field_Level::Field_Level() : Level()
{
	srand((unsigned int)time(NULL));

	//Background Color Assignment
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * HDX;
	background.body.height = 5.0f * HDY;
	background.set_color(Assets::backgroundPalette.day);

	//filter
	filterBG.center.x = background.body.center.x;
	filterBG.center.y = background.body.center.y;
	filterBG.width = background.body.width;
	filterBG.height = background.body.height;
	filterBG.color = Assets::palette.white;
	filterBG.color.alpha = 255;

	gradientBG.LLBG.body.center.x = 0;
	gradientBG.LLBG.body.center.y = 0;
	gradientBG.LLBG.body.width = 4 * HDX;
	gradientBG.LLBG.body.height = 4 * HDY;

	gradientBG.LRBG.body.center.x = 0;
	gradientBG.LRBG.body.center.y = 0;
	gradientBG.LRBG.body.width = 4 * HDX;
	gradientBG.LRBG.body.height = 4 * HDY;

	gradientBG.TRBG.body.center.x = 0;
	gradientBG.TRBG.body.center.y = 0;
	gradientBG.TRBG.body.width = 4 * HDX;
	gradientBG.TRBG.body.height = 4 * HDY;

	gradientBG.TLBG.body.center.x = 0;
	gradientBG.TLBG.body.center.y = 0;
	gradientBG.TLBG.body.width = 4 * HDX;
	gradientBG.TLBG.body.height = 4 * HDY;

	//Sun Attributes
	sun.radius = (GLfloat)HDY;
	sun.color = Assets::palette.sun;
	sun.center.x = (GLfloat)HDX;
	sun.center.y = (GLfloat)HDY;

	Platform p;
	platform.push_back(p);

	//Floor Center
	platform.at(0).body.center.x = 0.0;
	platform.at(0).body.center.y = HDY / -3.0f;
	platform.at(0).outline.center.x = platform.at(0).body.center.x;
	platform.at(0).outline.center.y = platform.at(0).body.center.y;

	//Floor Dimensions
	platform.at(0).body.width = 5.0f * HDX / 6.0f;
	platform.at(0).body.height = HDY / 20.0f;
	platform.at(0).outline.width = platform.at(0).body.width + THICKNESS;
	platform.at(0).outline.height = platform.at(0).body.height + THICKNESS;

	//Floor Boundaries - For Physics
	platform.at(0).body.boundary_assignment();

	//Assign Color to Floor
	platform.at(0).body.color = Assets::platformPalette.grass.body;
	platform.at(0).outline.color = Assets::platformPalette.grass.outline;

	//Floor Stroke Assignment
	platform.at(0).body.boundary_assignment();

	//Wind
	(rand() % 2 == 0) ? windDirection = RIGHT : windDirection = LEFT;

	for (int i = 0; i < MAX_CLOUDS; ++i)
		clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));

	// Initially only - Assign random X coordinate
	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) {
		it->get()->body[1].get()->center.x = rand() % (4 * HDX) - 2 * HDX;
		it->get()->body[0].get()->center.x = it->get()->body[1].get()->center.x - it->get()->body[1].get()->radius;
		it->get()->body[2].get()->center.x = it->get()->body[1].get()->center.x + it->get()->body[1].get()->radius;
	}
}

void Field_Level::update_clouds()
{
	GLfloat arg1, arg2;
	int i = 0;

	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) {
		// Parameters to determine when a cloud is off screen : 
		// Direction: Left, Anlyze position of Right subcloud
		arg1 = it->get()->body[2].get()->center.x + (it->get()->body[2].get()->radius);
		arg2 = it->get()->body[0].get()->center.x - (it->get()->body[0].get()->radius);

		//Reset if Last Cloud Offscreen
		if ((arg1 < -2.0f * HDX && windDirection == LEFT) 
			|| (arg2 > 2.0f * HDX && windDirection == RIGHT)) 
			it->get()->offScreen ^= 1;;

		it->get()->update(windDirection);

		++i;
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

	map<unsigned int, shared_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {		
		if (it->second->stats.lifeState == ALIVE)
			it->second->render();
	}
}

void Field_Level::phys_handler()
{
	update_clouds();
	purge_clouds();

	map<unsigned int, shared_ptr<Player>>::iterator it;

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
	filterBG.render();
	background.render();
	//gradientBG.render();

	sun.render();

	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) 
		it->get()->render();
	
	platform.at(0).outline.render();
	platform.at(0).body.render();
}

Night_Level::Night_Level() : Level()
{
	Platform p0, p1, p2, p3;
	platform.push_back(p0);
	platform.push_back(p1);
	platform.push_back(p2);
	platform.push_back(p3);

	//Background Attributes
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * HDX;
	background.body.height = 5.0f * HDY;

	//Color assignment
	background.set_color(Assets::backgroundPalette.night);

	//Moon Attributes
	moon.color = Assets::palette.moon;
	moon.center.x = HDX / 2.0f;
	moon.center.y = HDY / 2.0f;
	moon.radius = HDY / 2.0f;

	//Platform Horizontal Center
	platform.at(0).body.center.x = 0;
	platform.at(1).body.center.x = -750;
	platform.at(2).body.center.x = 0;
	platform.at(3).body.center.x = 750;

	//Platform Dimensions
	platform.at(0).body.width = 5.0f * HDX / 6.0f;
	platform.at(0).body.height = HDY / 20.0f;
	platform.at(0).body.center.y = HDY / -3.0f;

	for (unsigned int i = 1; i < platform.size(); i++) {
		platform.at(i).body.width = HDX / 4.0f;
		platform.at(i).body.height = HDY / 20.0f;
		platform.at(i).body.center.y = 100.0f;
	}

	platform.at(2).body.center.y = HDY / 4.0f;

	for (unsigned int i = 0; i < platform.size(); i++) {
		//Floor Boundaries - For Physics
		platform.at(i).body.boundary_assignment();

		//outline centered at body
		platform.at(i).outline.center.x = platform.at(i).body.center.x;
		platform.at(i).outline.center.y = platform.at(i).body.center.y;

		platform.at(i).outline.width = platform.at(i).body.width + THICKNESS;
		platform.at(i).outline.height = platform.at(i).body.height + THICKNESS;

		//Assign Color to Floor
		platform.at(i).body.color = Assets::platformPalette.grass.body;
		platform.at(i).outline.color = Assets::platformPalette.grass.outline;

		//Assign Floor Outline Attributes
		platform.at(i).body.boundary_assignment();
	}
}

void Night_Level::gfx_handler()
{
	//Dynamic Camera
	glOrtho(Camera::edges.left, Camera::edges.right, Camera::edges.bottom, Camera::edges.top, -1, 1);

	render();

	map<unsigned int, shared_ptr<Player>>::iterator it;

	for (it = playerMap.begin(); it != playerMap.end(); ++it) {
		if (it->second->stats.lifeState == ALIVE)
			it->second->render();
	}
}

void Night_Level::phys_handler()
{
	map<unsigned int, shared_ptr<Player>>::iterator it;

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

	for (unsigned int i = 0; i < platform.size(); i++) {
		platform.at(i).outline.render();
		platform.at(i).body.render();
	}
}

Time_Level::Time_Level() : Level()
{
	srand((unsigned int)time(NULL));

	//Background Attribute Assignment
	timeOfDay = EVENING;
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * HDX;
	background.body.height = 5.0f * HDY;

	bg_pal = Assets::backgroundPalette;

	transition = false;

	//Initialize Background
	background.set_color(Assets::backgroundPalette.evening);

	//Sun Attributes
	sun.radius = (GLfloat)HDY;
	sun.color = Assets::palette.sun;
	sun.center.x = (GLfloat)HDX;
	sun.center.y = (GLfloat)HDY;

	//Moon Attributes
	moon.color = Assets::palette.moon;
	moon.center.x = HDX / 2.0f;
	moon.center.y = HDY / 2.0f;
	moon.radius = HDY / 2.0f;

	Platform p;
	platform.push_back(p);

	//Floor Dimensions
	platform.at(0).body.width = 5.0f * HDX / 6.0f;
	platform.at(0).body.height = HDY / 20.0f;
	platform.at(0).body.center.x = 0.0f;
	platform.at(0).body.center.y = HDY / -3.0f;

	//Floor Boundaries - For Physics
	platform.at(0).body.boundary_assignment();

	//Assign Color to Floor
	platform.at(0).body.color = Assets::palette.platform;
	platform.at(0).outline.color = Assets::palette.black;

	//Floor Stroke Assignment
	platform.at(0).body.boundary_assignment();

	//Wind
	(rand() % 2 == 0) ? windDirection = RIGHT : windDirection = LEFT;

	for (int i = 0; i < MAX_CLOUDS; ++i)
		clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));

	// Initially only Assign random X coordinate
	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) {
		int randX = rand() % (4 * HDX) - 2 * HDX; // get random position for cloud
		it->get()->body[1].get()->center.x = randX;
		it->get()->body[0].get()->center.x = randX - it->get()->body[1].get()->radius;
		it->get()->body[2].get()->center.x = randX + it->get()->body[1].get()->radius;
	}
}

void Time_Level::update_clouds()
{
	GLfloat arg1, arg2;
	int i = 0;

	vector<unique_ptr<Cloud>>::iterator it;

	for (it = clouds.begin(); it != clouds.end(); ++it) {
		// Parameters to determine when a cloud is off screen
		arg1 = it->get()->body[1].get()->center.x + (it->get()->body[1].get()->radius * 3);
		arg2 = it->get()->body[1].get()->center.x - (it->get()->body[1].get()->radius * 3);

		//Reset if Last Cloud Offscreen
		if (arg1 < (-1.0f * HDX) && windDirection == LEFT) {
			clouds.erase(clouds.begin() + i);
			clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));
		}
		else if (arg2 > HDX && windDirection == RIGHT) {
			clouds.erase(clouds.begin() + i);
			clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));
		}

		it->get()->update(windDirection);

		++i;
	}
}

void Time_Level::purge_clouds()
{
	for (unsigned int i = 0; i < clouds.size(); ++i) {
		if (clouds[i].get()->offScreen) {
			clouds.erase(clouds.begin() + i);
			clouds.push_back(make_unique<Cloud>(Cloud().make_cloud(windDirection)));
		}
	}
}

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

	map<unsigned int, shared_ptr<Player>>::iterator it;
	
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
	
	map<unsigned int, shared_ptr<Player>>::iterator it;

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

	platform.at(0).body.render();
	platform.at(0).outline.render();
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