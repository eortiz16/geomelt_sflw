#pragma once

#include "headers.h"
#include "generic.h"
#include "primitives.h"
#include "player.h"

constexpr auto FAC1 = 1.0000001f;
constexpr auto FAC2 = 0.9999999f;
constexpr auto left_ortho = -0.75f * HDX;
constexpr auto right_ortho = 0.75f * HDX;
constexpr auto top_ortho = 0.75f * HDY;
constexpr auto bottom_ortho = -0.75f * HDY;

class Player;

class Camera {
private:
	float ZOOM;
	float aspect_ratio;
	float xMin;
	float xMax;
	float yMin;
	float yMax;

	float competitionXLeft;
	float competitionXRight;

	medmelt::Boundary edges;
	medmelt::Boundary ortho;
	Vec center;

	friend class Level;
	friend class Field_Level;
	friend class Night_Level;
	friend class Time_Level;
	friend class CharacterSelect;
public:
	// void update_value();
	void set_center(map<unsigned int, unique_ptr<Player>>& playerMap);
	void set_edges();
	void transition();
	Camera();
};
