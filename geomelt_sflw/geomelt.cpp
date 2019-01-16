#include "geomelt.h"

int main()
{
	Game game;
	game.loop();
	return EXIT_SUCCESS;
}

Game::Game()
{
	current.render = MENU;

	// Request a 24-bits depth buffer when creating the window
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;

	window = unique_ptr<sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode(HDX, HDY), "Geometric Meltdown", sf::Style::Default, contextSettings));
	//window = unique_ptr<RenderWindow> (new sf::RenderWindow(sf::VideoMode(HDX, HDY), "Geometric Meltdown", sf::Style::Fullscreen, contextSettings));

	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(FPS);

	current.menuChange = false;
	current.menu = MAINMENU;
	menu = unique_ptr<Menu>(new MainMenu(assets));

	// Active for OPENGL
	window->setActive(true);

	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, window->getSize().x, window->getSize().y);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DITHER);

	window->setActive(false);

	sync.previous = sync.game_clock.getElapsedTime().asMilliseconds();
}

Game::~Game()
{
	menu.reset();
	level.reset();
	window.reset();
}

Sync::Sync()
{
	game_clock.restart();
	lag = 0.0;
	current = 0.0;
	elapsed = 0.0;
	previous = 0.0;
}

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
				menu->read_axis(event.joystickButton.joystickId, assets);
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
					menu->read_buttons(event, assets, current, window);
					break;
				case CHARSEL:
					menu->read_buttons(event, assets, current, level);
					break;
				case LEVELSEL:
					menu->read_buttons(event, assets, current, level);
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

void CharacterSelect::read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level)
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
	{
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end()) { //if exists
			level->playerMap[event.joystickButton.joystickId].reset();
			//Sleep(1);
			level->playerMap[event.joystickButton.joystickId] = unique_ptr<Player>(new Boxy(assets));
		}
	}
		break;
	case Y:
	{
		cout << "Y";
	}
		break;
	case LB:
	{
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end())
			level->playerMap[event.joystickButton.joystickId].get()->change_color(PREV);
	}
		break;
	case RB:
	{
		if (level->playerMap.find(event.joystickButton.joystickId) != level->playerMap.end())
			level->playerMap[event.joystickButton.joystickId].get()->change_color(NEXT);
	}
		break;
	case START:
	{
		level->add_player(event.joystickButton.joystickId, assets);
		selectBox[level->playerMap[event.joystickButton.joystickId].get()->myID].occupied = true;
	}
		break;
	case SELECT:
	{
		//menu.reset();
		current.menu = LEVELSEL;
		current.menuChange = true;
	}
		break;
	default:
		break;
	}
}

void Game::loop()
{
	while (window->isOpen())
	{
		sync.current = sync.game_clock.getElapsedTime().asMilliseconds();
		sync.elapsed = sync.current - sync.previous;
		sync.previous = sync.current;
		sync.lag += sync.elapsed;

		process_input();

		while (sync.lag >= MS_PER_UPDATE())
		{
			if (current.render == LEVEL)
				level->phys_handler(assets, &camera);
			sync.lag -= MS_PER_UPDATE();
		}

		window->setActive(true);

		switch (current.render)
		{
		case MENU:
			if (current.menuChange == false)
			{
				switch (current.menu)
				{
				case MAINMENU:
				{
					menu->handler();
				}
				break;
				case CHARSEL:
				{
					menu->handler(camera, assets, level->playerMap.begin(), level->playerMap.end());
				}
				break;
				case LEVELSEL:
				{
					menu->handler();
				}
				break;
				default:
					break;
				}
			}
			break;
		case LEVEL:
			level->gfx_handler(camera);
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
				menu = unique_ptr<Menu>(new MainMenu(assets));
				break;
			case CHARSEL:
				menu = unique_ptr<Menu>(new CharacterSelect(assets));
				break;
			case LEVELSEL:
				menu = unique_ptr<Menu>(new LevelSelect(assets));
				break;
			case NONE:
				break;
			default:
				break;
			}
		}
	}
}
