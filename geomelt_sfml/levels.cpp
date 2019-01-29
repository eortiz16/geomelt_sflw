#include "levels.h"

PlayerMap Level::players;

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

void Level::render()
{
	blackVoid.render();
	background.render();
	scenery->render();
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
	Camera::set_center();
	Camera::set_edges();
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

	scenery = unique_ptr<SceneryGroup>(new FieldScenery);
}

void Field_Level::phys_handler()
{
	Level::phys_handler();
	scenery->physics();
}

void Field_Level::render()
{
	Level::render();
	platforms.render();
	players.render();
}

Night_Level::Night_Level() : Level()
{
	//Background Attributes
	background.body.center.x = 0;
	background.body.center.y = 0;
	background.body.width = 4.0f * SCRN_WD;
	background.body.height = 5.0f * SCRN_HT;
	background.set_color(Assets::backgroundPalette.night);

	scenery = unique_ptr<SceneryGroup>(new NightScenery);
}

void Night_Level::phys_handler()
{
	Level::phys_handler();
}

void Night_Level::render()
{
	Level::render();
	platforms.render();
	players.render();
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
	
	scenery = unique_ptr<SceneryGroup>(new TimeScenery);
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

void Time_Level::phys_handler() 
{
	Level::phys_handler();
	transition_handler();
	scenery->physics();
}

void Time_Level::render()
{
	Level::render();

	/*
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
	*/

	platforms.render();
	players.render();
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

	platforms.push_back(p0);
	platforms.push_back(p1);
	platforms.push_back(p2);
	platforms.push_back(p3);

	for (vector<Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it) {
		//Floor Boundaries - For Physics
		it->body.boundary_assignment();

		//outline centered at body
		it->outline.center = it->body.center;

		it->outline.width = it->body.width + THICKNESS;
		it->outline.height = it->body.height + THICKNESS;

		//Assign Color to Floor
		it->body.color = Assets::platformPalette.grass.body;
		it->outline.color = Assets::platformPalette.grass.outline;

		//Assign Floor Outline Attributes
		it->body.boundary_assignment();
	}
}