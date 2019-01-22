#include "sync.h"

Sync::Sync()
{
	game_clock.restart();
	lag = 0.0;
	current = 0.0;
	elapsed = 0.0;
	previous = 0.0;
}