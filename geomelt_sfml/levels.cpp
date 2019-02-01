#include "levels.h"

PlayerMap Level::players;

Level::Level()
{
	scenery = move(SceneryGroup::create((LevelType)(rand() % 3)));
}

void Level::render()
{
	scenery->render();
	platforms.render();
	players.render();
}

void Level::gfx_handler()
{
	//Dynamic Camera
	glOrtho(Camera::edges.left, Camera::edges.right, Camera::edges.bottom, Camera::edges.top, -1, 1);
	render();
}

void Level::phys_handler()
{
	players.phys_handler(platforms);
	scenery->physics();
	Camera::set_center();
	Camera::set_edges();
}

void Level::setScenery(SceneryGroup scenery)
{
	this->scenery = unique_ptr<SceneryGroup>(&scenery);
}

void PlatformGroup::render()
{
	for (vector<Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it) {
		it->outline.render();
		it->body.render();
	}
}

/* Platform[0] is the maain platform */
PlatformGroup::PlatformGroup()
{
	platforms.push_back(
		Platform(
			geomelt::Vec(0, -SCRN_HT * 2.5f, 0),
			SCRN_HT * 6.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	platforms.push_back(
		Platform(
			geomelt::Vec(0, SCRN_HT / -3.0f, 0),
			5.0f * SCRN_WD / 6.0f, 
			SCRN_HT / 20.0f, 
			Assets::platformPalette.grass
		)
	);

	platforms.push_back(
		Platform(
			geomelt::Vec(-SCRN_HT * 3.0f / 4.0f, 100.0f, 0),
			SCRN_WD / 4.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	platforms.push_back(
		Platform(
			geomelt::Vec(0.0f, SCRN_HT / 4.0f, 0),
			SCRN_WD / 4.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	platforms.push_back(
		Platform(
			geomelt::Vec(SCRN_HT * 3.0f / 4.0f, 100.0f, 0),
			SCRN_WD / 4.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	for (vector<Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it) 
		it->body.boundary_assignment();
}

Platform::Platform() {}

Platform::Platform(geomelt::Vec center, float width, float height, ColorSet color)
{
	this->body = geomelt::Quad(width, height, 0, color.body, center);
	this->outline = geomelt::Quad(width + THICKNESS, height + THICKNESS, 0, color.outline, center);
}

/* Great Candidate for State Design Pattern */
/*
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
*/