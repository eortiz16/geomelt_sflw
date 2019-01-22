#include "geomelt.h"

int main()
{
	Game game;

	return EXIT_SUCCESS;
}

Game::Game()
{
	this->gfxNet.loop();
}

/*
void Game::process_input()
{
	Player *plyr = NULL;

	sf::Event event;

	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
			{
				window->close();
			}
				break;
			default:
				break;
			}
			break;
		case sf::Event::JoystickMoved:
			if (current.render == LEVEL) {
				if (level->playerMap.find(event.joystickMove.joystickId) != level->playerMap.end()) {
					plyr = level->playerMap[event.joystickConnect.joystickId].get();
					plyr->read_axes(event.joystickConnect.joystickId);
				}
			}
			else if (current.render == MENU) {
				menu->read_axis(event.joystickButton.joystickId);
			}
			break;
		case sf::Event::JoystickButtonPressed:
			if (current.render == LEVEL)
			{
				if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end()) {
					plyr = level->playerMap[event.joystickButton.joystickId].get();
					plyr->read_buttons(event.joystickButton.button);
				}
			}
			else if (current.render == MENU)
				switch (current.menu)
				{
				case MAINMENU:
					if (menu->read_buttons(event, current) == -1)
						window->close();
					break;
				case CHARSEL:
					menu->read_buttons(event, current, level);
					break;
				case LEVELSEL:
					menu->read_buttons(event, current, level);
					break;
				default:
					break;
				}
			break;
		default:
			break;
		}
	}
}
*/
/*
void CharacterSelect::read_buttons(sf::Event event, CurrentGameState &current, unique_ptr<Level> &level)
{
	Player *plyr = NULL;
	map<unsigned int, unsigned int>::iterator it;

	switch (event.joystickButton.button)
	{
	case A:
		break;
	case B:
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end()) { //if exists
			plyr = level->playerMap[event.joystickButton.joystickId].get();
			selectBox[plyr->myID].occupied = false;
			plyr->stats.lifeState = ELIMINATED;
		}
		break;
	case X:
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end()) { //if exists
			level->playerMap[event.joystickButton.joystickId].reset();
			level->playerMap[event.joystickButton.joystickId] = unique_ptr<Player>(new Boxy());
		}
		break;
	case Y:
		cout << "Y";
		break;
	case LB:
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end())
			level->playerMap[event.joystickButton.joystickId].get()->change_color(PREV);
		break;
	case RB:
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end())
			level->playerMap[event.joystickButton.joystickId].get()->change_color(NEXT);
		break;
	case START:
		level->add_player(event.joystickButton.joystickId);
		selectBox[level->playerMap[event.joystickButton.joystickId].get()->myID].occupied = true;
		break;
	case SELECT:
		//menu.reset();
		current.menu = LEVELSEL;
		current.menuChange = true;
		break;
	default:
		break;
	}
}
*/

/*
void Game::loop()
{
	while (window->isOpen())
	{
		sync.current = sync.game_clock.getElapsedTime().asMilliseconds();
		sync.elapsed = sync.current - sync.previous;
		sync.previous = sync.current;
		sync.lag += sync.elapsed;

		//process_input();

		/*
		while (sync.lag >= MS_PER_UPDATE())
		{
			if (current.render == LEVEL)
				level->phys_handler();
			sync.lag -= MS_PER_UPDATE();
		}

		window->setActive(true);

		gfxNet._state->handler();

		switch (current.render)
		{
		case MENU:
			if (current.menuChange == false)
			{
				switch (current.menu)
				{
				case MAINMENU:
				case LEVELSEL:
				{
					menu->handler();
				}
				break;
				case CHARSEL:
				{
					menu->handler(level->playerMap.begin(), level->playerMap.end());
				}
				break;
				default:
					break;
				}
			}
			break;
		case LEVEL:
			level->gfx_handler();
			break;
		default:
			break;
		}

		glClear(GL_DEPTH_BUFFER_BIT);

		// Transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		window->setActive(false);

		window->display();

		//CLEAN UP
		level->purge_players();
		if (current.menuChange)
		{
			current.menuChange = false;
			menu.reset();

			switch (current.menu)
			{
			case MAINMENU:
				menu = unique_ptr<Menu>(new MainMenu());
				break;
			case CHARSEL:
				menu = unique_ptr<Menu>(new CharacterSelect());
				break;
			case LEVELSEL:
				menu = unique_ptr<Menu>(new LevelSelect());
				break;
			case NONE:
				break;
			default:
				break;
			}
		}
		
	}
}
*/
