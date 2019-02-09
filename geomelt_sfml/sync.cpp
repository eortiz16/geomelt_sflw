#include "sync.h"

sf::Clock Sync::gameClock;

double Sync::previous = 0.0;
double Sync::current = 0.0;
double Sync::elapsed = 0.0;
double Sync::lag = 0.0;

void Sync::update()
{
	current = gameClock.getElapsedTime().asMilliseconds();
	elapsed = current - previous;
	previous = current;
	lag += elapsed;
}

void Sync::catch_up()
{
	while (lag >= MS_PER_UPDATE()) {
		//if (typeid(*_state) == typeid(LevelState))
			//level.phys_handler();

		lag -= MS_PER_UPDATE();
	}
}

Sync::Sync()
{
	gameClock.restart();
}