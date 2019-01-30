#include "sync.h"

void Sync::update()
{
	current = game_clock.getElapsedTime().asMilliseconds();
	elapsed = current - previous;
	previous = current;
	lag += elapsed;
}

void Sync::catch_up()
{
	while (lag >= MS_PER_UPDATE()) {
		//if (typeid(*_state) == typeid(LevelState))
			//level->phys_handler();

		lag -= MS_PER_UPDATE();
	}
}

Sync::Sync()
{
	game_clock.restart();
	lag = 0.0;
	current = 0.0;
	elapsed = 0.0;
	previous = 0.0;
}