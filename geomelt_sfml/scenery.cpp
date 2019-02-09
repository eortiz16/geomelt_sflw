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

void Stars::render()
{
	stars.render();
}

void Stars::physics()
{
	stars.update();
}

void Clds::render()
{
	clouds.render();
}

void Clds::physics()
{
	clouds.update();
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

void SceneryGroup::addObject(unique_ptr<Scenery>& obj)
{
	scenery.push_back(move(obj));
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
	unique_ptr<Scenery> blackVoid = unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.black,
			Quad(10.0f * SCRN_WD, 10.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
	);

	addObject(blackVoid);
	blackVoid.release();
}

FieldScenery::FieldScenery() : SceneryGroup() 
{
	unique_ptr<Scenery> background = unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.day,
			Quad(4.0f * SCRN_WD, 5.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
	);

	addObject(background);
	background.release();

	unique_ptr<Scenery> sun = unique_ptr<Scenery>(new Sun);
	addObject(sun);
	sun.release();

	unique_ptr<Scenery> clouds = unique_ptr<Scenery>(new Clds);
	addObject(clouds);
	clouds.release();
}

NightScenery::NightScenery() : SceneryGroup() {
	unique_ptr<Scenery> background = unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.night,
			Quad(4.0f * SCRN_WD, 5.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
	);

	addObject(background);
	background.release();

	unique_ptr<Scenery> stars = unique_ptr<Scenery>(new Stars);
	addObject(stars);
	stars.release();

	unique_ptr<Scenery> moon = unique_ptr<Scenery>(new Moon);
	addObject(moon);
	moon.release();
}

TimeScenery::TimeScenery() : SceneryGroup() {
	unique_ptr<Scenery> background = unique_ptr<Scenery>(
		new Background(
			Assets::backgroundPalette.evening,
			Quad(4.0f * SCRN_WD, 5.0f * SCRN_HT, 0.0f, Assets::palette.black, Vec(0, 0, 0))
		)
	);

	addObject(background);
	background.release();

	unique_ptr<Scenery> stars = unique_ptr<Scenery>(new Stars);
	addObject(stars);
	stars.release();

	unique_ptr<Scenery> moon = unique_ptr<Scenery>(new Moon);
	addObject(moon);
	moon.release();

	unique_ptr<Scenery> clouds = unique_ptr<Scenery>(new Clds);
	addObject(clouds);
	clouds.release();
}
