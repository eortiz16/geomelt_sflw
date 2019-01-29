#include "scenery.h"

void Sky::render()
{
}

void Sky::physics()
{
}

Sky::Sky()
{
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
	body = geomelt::Circle(
		SCRN_HT,
		Assets::palette.sun,
		geomelt::Vec(SCRN_WD, SCRN_HT, 0)
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
	body = geomelt::Circle(
		SCRN_HT / 2.0f,
		Assets::palette.moon,
		geomelt::Vec(SCRN_WD / 2.0f, SCRN_HT / 2.0f, 0)
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

FieldScenery::FieldScenery() {
	unique_ptr<Scenery> sun = unique_ptr<Scenery>(new Sun);
	addObject(sun);
	sun.release();

	unique_ptr<Scenery> clouds = unique_ptr<Scenery>(new Clds);
	addObject(clouds);
	clouds.release();
}

NightScenery::NightScenery() {
	unique_ptr<Scenery> stars = unique_ptr<Scenery>(new Stars);
	addObject(stars);
	stars.release();

	unique_ptr<Scenery> moon = unique_ptr<Scenery>(new Moon);
	addObject(moon);
	moon.release();
}

TimeScenery::TimeScenery() {

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
