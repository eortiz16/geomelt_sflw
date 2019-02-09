#pragma once

#include "headers.h"
#include "player.h"

constexpr auto FAC1 = 1.0000001f;
constexpr auto FAC2 = 0.9999999f;
constexpr auto left_ortho = -0.75f * SCRN_WD;
constexpr auto right_ortho = 0.75f * SCRN_WD;
constexpr auto top_ortho = 0.75f * SCRN_HT;
constexpr auto bottom_ortho = -0.75f * SCRN_HT;

class Camera {
private:
	static float ZOOM;
	static float aspect_ratio;
	static float xMin;
	static float xMax;
	static float yMin;
	static float yMax;

	static float competitionXLeft;
	static float competitionXRight;

	static Boundary edges;
	static Boundary ortho;
	static Vec center;
public:
	// void update_value();
	static void set_center();
	static void set_edges();
	static void transition();

	Camera() {}

	friend class Boundary;
	friend class Level;
	friend class CharacterSelect;
	friend class Menu;
	friend class MainMenu;
	friend class CharacterSelect;
	friend class LevelSelect;
	friend class Pause;
};
