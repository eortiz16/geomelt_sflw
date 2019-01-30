#pragma once

#include <algorithm>
#include <iostream>
#include <cmath>
#include <ctime>
#include <climits>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

#ifndef GL_SRGB8_ALPHA8
	#define GL_SRGB8_ALPHA8 0x8C43
#endif

enum Direction { LEFT, RIGHT };
constexpr auto CORNERS = 4;

/*
constexpr auto SCRN_WD = 3440;
constexpr auto SCRN_HT = 1440;
*/

constexpr auto SCRN_WD = 1920;
constexpr auto SCRN_HT = 1080;

/*
constexpr auto SCRN_WD = 1366;
constexpr auto SCRN_HT = 720;
*/