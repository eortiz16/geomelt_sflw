#pragma once

#include "headers.h"
#include "generic.h"
#include "primitives.h"
#include "player.h"

#define FAC1 1.0000001f
#define FAC2 0.9999999f

class Player;

class Camera {
public:
	float ZOOM;
	float aspect_ratio;
	float xMin;
	float xMax;
	float yMin;
	float yMax;

	float competitionXLeft;
	float competitionXRight;
	// void update_value();

	Boundary_ edges;
	Boundary_ ortho;
	Vec center;
	void set_center(map<unsigned int, unique_ptr<Player>>& playerMap);
	void set_edges();
	void transition();
	Camera();
};

static float left_ortho = -0.75f * HDX;
static float right_ortho = 0.75f * HDX;
static float top_ortho = 0.75f * HDY;
static float bottom_ortho = -0.75f * HDY;