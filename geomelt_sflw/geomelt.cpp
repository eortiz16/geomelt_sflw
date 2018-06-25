#include "geomelt.h"

int main()
{
	Game game;

	// Active for OPENGL
	game.window->setActive(true);

	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, game.window->getSize().x, game.window->getSize().y);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DITHER);

	game.window->setActive(false);

	game.sync.previous = game.sync.game_clock.getElapsedTime().asMilliseconds();

	while (game.window->isOpen())
	{
		game.sync.current = game.sync.game_clock.getElapsedTime().asMilliseconds();
		game.sync.elapsed = game.sync.current - game.sync.previous;
		game.sync.previous = game.sync.current;
		game.sync.lag += game.sync.elapsed;
		
		game.input.process(&game);

		while (game.sync.lag >= MS_PER_UPDATE)
		{
			if (game.render == LEVEL)
				game.level->phys_handler(game.assets, &game.camera);
			game.sync.lag -= MS_PER_UPDATE;
		}

		game.window->setActive(true);

		switch (game.render)
		{
			case MENU:
				if (game.menuChange == false) 
				{
					switch (game.currentMenu)
					{
					case MAINMENU:
					{
						game.currentMenu = MAINMENU;
						game.menu->handler();
					}
						break;
					case CHARSEL:
					{
						game.currentMenu = CHARSEL;
						game.menu->handler(game.camera, game.assets, game.level.get()->playerMap.begin(), game.level.get()->playerMap.end());
					}
						break;
					case LEVELSEL:
					{
						game.currentMenu = LEVELSEL;
					}
					break;
					default:
						break;
					}
				}
				break;
			case LEVEL:
				game.level->gfx_handler(game.camera);
				break;
		}

		glClear(GL_DEPTH_BUFFER_BIT);

		// Transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		game.window->setActive(false);

		game.window->display();

		//CLEAN UP
		game.level->purge_players();
		if (game.menuChange)
		{
			game.menuChange = false;
			game.menu.reset();

			switch (game.currentMenu)
			{
			case MAINMENU:
				game.menu = unique_ptr<Menu>(new MainMenu(game.assets));
				break;
			case CHARSEL:
				game.menu = unique_ptr<Menu>(new CharacterSelect(game.assets));
				break;
			case LEVELSEL:
				game.menu = unique_ptr<Menu>(new LevelSelect(game.assets));
				break;
			case NONE:
				break;
			default:
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}

Game::Game()
{
	render = MENU;

	// Request a 24-bits depth buffer when creating the window
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;

	window = unique_ptr<RenderWindow> (new RenderWindow(VideoMode(HDX, HDY), "Geometric Meltdown", Style::Default, contextSettings));
	//window = unique_ptr<RenderWindow> (new RenderWindow(VideoMode(HDX, HDY), "Geometric Meltdown", Style::Fullscreen, contextSettings));

	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(FPS);

	menuChange = false;
	currentMenu = MAINMENU;
	menu = unique_ptr<Menu>(new MainMenu(assets));
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

void Input::process(Game *game)
{
	Player *plyr = NULL;

	Event event;

	while (game->window->pollEvent(event))
	{
		switch (event.type)
		{
		case Event::KeyPressed:
			switch (event.key.code)
			{
			case Keyboard::Escape:
			{
				game->window->close();
			}
				break;
			}
			break;
		case Event::JoystickMoved:
			if (game->render == LEVEL)
			{
				if (game->level->playerMap.find(event.joystickMove.joystickId) != game->level->playerMap.end())
				{
					plyr = game->level->playerMap[event.joystickConnect.joystickId].get();
					plyr->read_axes(event.joystickConnect.joystickId);
				}
			}
			else if (game->render == MENU)
			{
				game->menu->read_axis(event.joystickButton.joystickId, game->assets);
			}
			break;
		case Event::JoystickButtonPressed:
			if (game->render == LEVEL)
			{
				if (game->level->playerMap.find(event.joystickButton.joystickId) != game->level->playerMap.end())
				{
					plyr = game->level->playerMap[event.joystickButton.joystickId].get();
					plyr->read_buttons(event.joystickButton.button);
				}
			}
			else if (game->render == MENU)
				switch (game->currentMenu)
				{
				case MAINMENU:
					game->menu->read_buttons(event.joystickButton.button, game->assets, &game->currentMenu, &game->menuChange);
					break;
				case CHARSEL:
					game->menu->read_buttons(event.joystickButton.joystickId, event.joystickButton.button, game);
					break;
				case LEVELSEL:
					game->menu->read_buttons(event.joystickButton.button, &game->render, &game->currentMenu, &game->menuChange, game->level, game->assets);
					break;
				}
			break;
		}
	}
}

void CharacterSelect::read_buttons(unsigned int joyID, unsigned int button, Game *game)
{
	Player *plyr = NULL;
	map<unsigned int, unsigned int>::iterator it;

	switch (button)
	{
	case A:
		break;
	case B:
		if (game->level->playerMap.find(joyID) != game->level->playerMap.end()) //if exists
		{
			plyr = game->level->playerMap[joyID].get();
			plyr->stats.lifeState = ELIMINATED;
		}
		break;
	case X:
	{
		if (game->level->playerMap.find(joyID) != game->level->playerMap.end()) //if exists
		{
			map <unsigned int, unique_ptr<Player>>::iterator it;
			it = game->level->playerMap.find(joyID);
			it->second.reset();
			Sleep(1);
			it->second = unique_ptr<Player>(new Boxy(game->assets));
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
		if (game->level->playerMap.find(joyID) != game->level->playerMap.end())
			game->level->playerMap[joyID].get()->change_color(PREV);
	}
		break;
	case RB:
	{
		if (game->level->playerMap.find(joyID) != game->level->playerMap.end())	
			game->level->playerMap[joyID].get()->change_color(NEXT);
	}
		break;
	case START:
	{
		game->level->add_player(joyID, game->assets);
	}
		break;
	case SELECT:
	{
		game->menu.reset();
		game->currentMenu = LEVELSEL;
		game->menuChange = true;
	}
		break;
	default:
		break;
	}
}