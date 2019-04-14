#include "scenery.h"

void Background::render()
{
	glBegin(GL_QUADS);
	glColor4ub((GLubyte)color[0].r, (GLubyte)color[0].g, (GLubyte)color[0].b, 255);
	glVertex2f(body.center.x - body.width / 2, body.center.y - body.height / 2);
	glColor4ub((GLubyte)color[1].r, (GLubyte)color[1].g, (GLubyte)color[1].b, 255);
	glVertex2f(body.center.x - body.width / 2, body.center.y + body.height / 2);
	glColor4ub((GLubyte)color[2].r, (GLubyte)color[2].g, (GLubyte)color[2].b, 255);
	glVertex2f(body.center.x + body.width / 2, body.center.y + body.height / 2);
	glColor4ub((GLubyte)color[3].r, (GLubyte)color[3].g, (GLubyte)color[3].b, 255);
	glVertex2f(body.center.x + body.width / 2, body.center.y - body.height / 2);
	glEnd();
}

void Background::physics()
{
}

Background::Background()
{
	for (int i = 0; i < CORNERS; i++) {
		color.push_back(Color());
		transition_done.push_back(false);
	}
}

Background::Background(vector<Color> clr, Quad qd) : Background()
{
	this->body = qd;
	this->color = clr;
}

void Satelite::render()
{
	this->body.render();
}

void Sun::render()
{
	Satelite::render();
}

void Sun::physics()
{
}

Sun::Sun()
{
	body = Circle(
		SCRN_HT,
		Assets::palette.sun,
		Vec(SCRN_WD, SCRN_HT, 0)
	);
}

void Moon::render()
{
	Satelite::render();
}

void Moon::physics()
{
}

Moon::Moon()
{
	body = Circle(
		SCRN_HT / 2.0f,
		Assets::palette.moon,
		Vec(SCRN_WD / 2.0f, SCRN_HT / 2.0f, 0)
	);
}

Stars::Stars()
{
	for (unsigned int i = 0; i < MAX_STAR; i++)
		group.push_back(Star(i));
}

void Stars::render()
{
	for (vector<Star>::iterator it = group.begin(); it != group.end(); ++it) {
		it->change_color();
		it->body->render();
	}
}

void Stars::physics()
{
	//for moving accross the screen;
}

Clouds::Clouds()
{
	//Wind Direction
	(rand() % 2 == 0) ? windDirection = RIGHT : windDirection = LEFT;

	//Create MAX_CLOUDS amount of clouds
	for (int i = 0; i < MAX_CLOUDS; ++i) 
		group.push_back(Cloud::make_cloud(windDirection));

	// Initially only Assign random X coordinate
	for (auto &cloud : group) {
		int randX = rand() % (4 * SCRN_WD) - 2 * SCRN_WD; // get random position for cloud

		cloud.body[1].center.x = (float)randX;
		cloud.body[0].center.x = randX - cloud.body[1].radius;
		cloud.body[2].center.x = randX + cloud.body[1].radius;
	}
}

void Clouds::render()
{
	for (auto &cloud : group)
		cloud.render();
}

void Clouds::physics()
{
	GLfloat arg1, arg2;

	int offset = 0;

	for (vector<Cloud>::iterator it = group.begin(); it != group.end(); ++it) {
		// Parameters to determine when a cloud is off screen
		arg1 = it->body[1].center.x + (it->body[1].radius  * 1.5f);
		arg2 = it->body[1].center.x - (it->body[1].radius  * 1.5f);
		offset = it - group.begin();

		//Reset if Last Cloud Offscreen
		if (arg1 < -2.0f * SCRN_WD && windDirection == LEFT) {
			group.erase(group.begin() + offset);
			group.push_back(Cloud::make_cloud(windDirection));
		}
		else if (arg2 > 2.0f * SCRN_WD && windDirection == RIGHT) {
			group.erase(group.begin() + offset);
			group.push_back(Cloud::make_cloud(windDirection));
		}

		it->update(windDirection);
	}
}

void Flowers::render()
{
}

void Flowers::physics()
{
}

void SceneryGroup::render()
{
	for (auto &element : scenery) 
		element->render();
}

void SceneryGroup::physics()
{
	for (auto &element : scenery)
		element->physics();
}

unique_ptr<SceneryGroup> SceneryGroup::create(LevelType lvl)
{
	switch (lvl) {
	case FIELD:
		return unique_ptr<SceneryGroup>(new FieldScenery);
		break;
	case NIGHT:
		return unique_ptr<SceneryGroup>(new NightScenery);
		break;
	case TIME:
		return unique_ptr<SceneryGroup>(new TimeScenery);
		break;
	default:
		return NULL;
		break;
	}
}

SceneryGroup::SceneryGroup()
{
	cout << "!";
	scenery.push_back(move(unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.black,
			Quad(10.0f * SCRN_WD, 10.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
		)));
}

FieldScenery::FieldScenery() : SceneryGroup() 
{
	scenery.push_back(move(unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.day,
			Quad(4.0f * SCRN_WD, 5.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
		)));

	scenery.push_back(move(unique_ptr<Scenery>(new Sun)));
	scenery.push_back(move(unique_ptr<Scenery>(new Clouds)));
}

NightScenery::NightScenery() : SceneryGroup() {
	scenery.push_back(move(unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.evening,
			Quad(4.0f * SCRN_WD, 5.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
		)));

	scenery.push_back(move(unique_ptr<Scenery>(new Stars)));
	scenery.push_back(move(unique_ptr<Scenery>(new Moon)));
}

TimeScenery::TimeScenery() : SceneryGroup() {
	scenery.push_back(move(unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.evening,
			Quad(4.0f * SCRN_WD, 5.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
		)));

	scenery.push_back(move(unique_ptr<Scenery>(new Stars)));
	scenery.push_back(move(unique_ptr<Scenery>(new Moon)));
	scenery.push_back(move(unique_ptr<Scenery>(new Clouds)));
}
