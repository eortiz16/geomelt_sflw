#include "levels.h"

PlayerMap Level::_players;

Level::Level()
{
	_scenery = move(SceneryGroup::create((LevelType)(rand() % 3)));
}

void Level::render()
{
	_scenery->render();
	_platforms.render();
	_players.render();
}

void Level::gfx_handler()
{
	//Dynamic Camera
	glOrtho(Camera::_edges.left, Camera::_edges.right, Camera::_edges.bottom, Camera::_edges.top, -1, 1);
	render();
}

void Level::phys_handler()
{
	_players.phys_handler(_platforms);
	_scenery->physics();
	Camera::set_center();
	Camera::set_edges();
}

void Level::setScenery(SceneryGroup scenery)
{
	this->_scenery = unique_ptr<SceneryGroup>(&scenery);
}

void PlatformGroup::render()
{
	for (vector<Platform>::iterator it = _platforms.begin(); it != _platforms.end(); ++it) {
		it->_outline.render();
		it->_body.render();
	}
}

/* Platform[0] is the main platform */
PlatformGroup::PlatformGroup()
{
	_platforms.push_back(
		Platform(
			Vec(0, -SCRN_HT * 2.5f, 0),
			SCRN_HT * 6.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	_platforms.push_back(
		Platform(
			Vec(0, SCRN_HT / -3.0f, 0),
			5.0f * SCRN_WD / 6.0f, 
			SCRN_HT / 20.0f, 
			Assets::platformPalette.grass
		)
	);

	_platforms.push_back(
		Platform(
			Vec(-SCRN_HT * 3.0f / 4.0f, 100.0f, 0),
			SCRN_WD / 4.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	_platforms.push_back(
		Platform(
			Vec(0.0f, SCRN_HT / 4.0f, 0),
			SCRN_WD / 4.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	_platforms.push_back(
		Platform(
			Vec(SCRN_HT * 3.0f / 4.0f, 100.0f, 0),
			SCRN_WD / 4.0f,
			SCRN_HT / 20.0f,
			Assets::platformPalette.grass
		)
	);

	for (vector<Platform>::iterator it = _platforms.begin(); it != _platforms.end(); ++it) 
		it->_body.boundary_assignment();
}

Platform::Platform() {}

Platform::Platform(Vec center, float width, float height, ColorSet color)
{
	this->_body = Quad(width, height, 0, color.body, center);
	this->_outline = Quad(width + THICKNESS, height + THICKNESS, 0, color.outline, center);
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
void Time_Level::transition_to(Color *clr)
{
	//Transition from bg to clr
	Color *bg;

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