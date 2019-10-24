#include "levels.h"

PlayerMap Level::_players;

Level::Level()
{
	_scenery = move(SceneryGroup::create((LevelType)(rand() % 3)));
	_overlay = Overlay(this->_players);
}

void Level::render()
{
	_scenery->render();
	_platforms.render();
	_players.render();
	_overlay.render(_players);
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

void Level::event_handler()
{
	// Check all players that are attacking
	for (auto itr = _players._map.begin(); itr != _players._map.end(); itr++) {
		if (itr->second->toggle.attacking) {
			//Check is arm is within the boundaries of other players
			for (auto jtr = _players._map.begin(); jtr != _players._map.end(); jtr++) {
				if (itr != jtr) {
					if (itr->second->Attack(*jtr->second)) {
						jtr->second->attackedBy(*itr->second);
					}
				}
			}
		}
	}
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

StatBox::StatBox(const shared_ptr<Player>& p)
{
	this->player = p;

	// Set the box's attributes
	body = Quad(
		350,
		350,
		350 / 2.0f,
		Color(255, 255, 255,75),
		Vec(0, 0, 0)
	);

}

void StatBox::update()
{
	//Todo: maintain aspect ratio of each char and adjust accordingly
	this->player->stats.health; // to display damgae incurred

	//obtain center of camera
	float apectratio = 0.0f;
	
	//Scale the proportions
	apectratio = 1.0f; // 1:1
	
	//update the position of the box (center)
	body.height = (Camera::_edges.top - Camera::_edges.bottom) * 0.20f;
	body.width = body.height * apectratio;

	//Partition screen into n + 1 parts (since we want to display 4 players info, 5 parts)
	float offset = (abs(Camera::_edges.left) + abs(Camera::_edges.right)) / 5.0f;

	//incorporate offset from left edge
	float centerX = Camera::_edges.left + (offset * (player->myID + 1));

	//offset from the bottom
	float centerY = Camera::_edges.bottom + body.height;
	body.center = Vec(centerX, centerY, 0);
	body.boundary_assignment();
}

void StatBox::render()
{
	this->update();
	this->body.render();
}

Overlay::Overlay(PlayerMap players)
{
	// TODO: choose Default direction
	for (auto itr = players._map.begin(); itr != players._map.end(); ++itr) {
		statBox.push_back(StatBox(itr->second));
	}
}

void Overlay::render(PlayerMap players)
{
	// TODO: if any player within any box, make that box disapear
	for (auto &box : statBox) {
		bool changeOpacity = true;

		for (auto &player : players._map) {
			int x = player.second->body->center.x, y = player.second->body->center.y;

			bool cond = false;
			if ((x <= box.body.center.x + box.body.width / 2) &&
				(x >= box.body.center.x - box.body.width / 2) &&
				(y <= box.body.center.y + box.body.width / 2) &&
				(y >= box.body.center.y - box.body.width / 2)) {
				changeOpacity = false;
				break;
			}
		}
		
		//TODO: transition to opacity do not instantly change it
		if (changeOpacity) 
			box.body.color = Color(255, 255, 255, 75);
		else 
			box.body.color = Color(255, 255, 255, 0);

		box.render();
	}
}
