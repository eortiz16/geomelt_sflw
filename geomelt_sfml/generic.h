#pragma once

#include "headers.h"

enum RenderState { MM, CSEL, LSEL, PAS, LVL };
enum Direction { LEFT, RIGHT };

constexpr auto CORNERS = 4;
//#define HDX 3440
//#define HDY 1440
constexpr auto HDX = 1920;
constexpr auto HDY = 1080;
//#define HDX 1366
//#define HDY 720