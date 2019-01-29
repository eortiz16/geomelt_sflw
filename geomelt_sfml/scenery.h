#pragma once
#include "headers.h"
#include "primitives.h"
#include "complex_shapes.h"

class Scenery {
public:
	virtual void render() = 0;
	virtual void physics() = 0;
};

class Sky : public Scenery {
public:
	void render();
	void physics();
	Sky();
};


class Satelite : public Scenery {
protected:
	geomelt::Circle body;
public:
	virtual void render();
	virtual void physics() = 0;
};

class Sun : public Satelite {
public:
	void render();
	void physics();
	Sun();
};

class Moon : public Satelite {
public:
	void render();
	void physics();
	Moon();
};

class Stars : public Scenery {
private:
	StarGroup stars;
public:
	void render();
	void physics();
};

class Flowers : public Scenery {
	vector<TexturedQuad> bodies;
public:
	void render();
	void physics();
};

class Clds : public Scenery {
private:
	CloudGroup clouds;
public:
	void render();
	void physics();
};

/* Empty SceneryGroup */
class SceneryGroup {
protected:
	vector<unique_ptr<Scenery>> scenery;
public:
	virtual void render();
	virtual void physics();
	void addObject(unique_ptr<Scenery>& obj);
};

/* Potential trajectort Below */
class FieldScenery : public SceneryGroup {
	FieldScenery();

	friend class Field_Level;
};

class NightScenery : public SceneryGroup {
	NightScenery();

	friend class Night_Level;
};

class TimeScenery : public SceneryGroup {
	TimeScenery();

	friend class Time_Level;
};