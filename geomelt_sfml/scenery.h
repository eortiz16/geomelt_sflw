#pragma once
#include "headers.h"
#include "primitives.h"
#include "complex_shapes.h"

enum LevelType { FIELD, NIGHT, TIME };

class Scenery {
public:
	virtual void render() = 0;
	virtual void physics() = 0;
};

class Background : public Scenery {
private:
	geomelt::Quad body;
	vector<geomelt::Color> color;
	vector<bool> transition_done;
public:
	void render();
	void physics();
	Background();
	Background(geomelt::Color * clr, geomelt::Quad qd);

	friend class SceneryGroup;
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
	static unique_ptr<SceneryGroup> create(LevelType lvl);

	SceneryGroup();
};

/* Potential trajectort Below */
class FieldScenery : public SceneryGroup {
	FieldScenery();

	friend class SceneryGroup;
};

class NightScenery : public SceneryGroup {
	NightScenery();

	friend class SceneryGroup;
};

class TimeScenery : public SceneryGroup {
	TimeScenery();

	friend class SceneryGroup;
};