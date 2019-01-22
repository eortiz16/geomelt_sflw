#include "state.h"

GFXNet::GFXNet()
{
	srand((unsigned int)time(NULL));

	// Request a 24-bits depth buffer when creating the window
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;
	contextSettings.antialiasingLevel = 16; //MAX

	// Default to Main Menu
	_state = new MainMenuState(this);

	// To draw the back ground
	level = (rand() % 2 == 0) ? unique_ptr<Level>(new Field_Level())
		: unique_ptr<Level>(new Night_Level());

	window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(SCRN_WD, SCRN_HT), "Geometric Meltdown", sf::Style::Default, contextSettings));
	//window = unique_ptr<sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode(SCRN_WD, SCRN_HT), "Geometric Meltdown", sf::Style::Fullscreen, contextSettings));

	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(FPS);

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

void GFXNet::loop()
{
	while (window->isOpen())
	{
		sync.current = sync.game_clock.getElapsedTime().asMilliseconds();
		sync.elapsed = sync.current - sync.previous;
		sync.previous = sync.current;
		sync.lag += sync.elapsed;

		_state->read_input();

		while (sync.lag >= MS_PER_UPDATE()) {
			//if (typeid(*_state) == typeid(LevelState))
				//level->phys_handler();

			sync.lag -= MS_PER_UPDATE();
		}

		window->setActive(true);

		//Render
		_state->handler();

		glClear(GL_DEPTH_BUFFER_BIT);

		// Transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		window->setActive(false);

		window->display();
	}
}

void GFXNet::setState(RState* state) 
{
	_state = state;
}

void GFXNet::next()
{
	_state->next();
}

void GFXNet::prev()
{
	_state->prev();
}

void GFXNet::handler()
{
	_state->handler();
}

//==================================================================

MainMenuState::MainMenuState(GFXNet* context) : _context(context) {}

void MainMenuState::next()
{
	_context->setState(new CharacterSelectState(_context));
}

void MainMenuState::prev() { /* Do nothing */ }

void MainMenuState::handler()
{
	menu.handler(_context->level);
}

void MainMenuState::read_input()
{
	sf::Event event;
	
	while (_context->window->pollEvent(event)) {
		switch (event.type)	{
		case sf::Event::Closed:
			_context->window->close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				menu.selected--;
				menu.update_selected();
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				menu.selected++;
				menu.update_selected();
				break;
			case sf::Keyboard::Return:
				if (menu.selected == PLAY)
					next();
				break;
			case sf::Keyboard::Escape:
				_context->window->close();
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case A:
			//case CROSS:
				if (menu.selected == PLAY)
					next();
				else if (menu.selected == EXIT)
					_context->window->close();
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickMoved:
		{
			float axis_position1 = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::PovY); //DPAD

			if (axis_position1 == 100) //up
			{
				menu.selected--;
				menu.update_selected();
			}
			else if (axis_position1 == -100) //down
			{
				menu.selected++;
				menu.update_selected();
			}
		}
			break;
		case sf::Event::MouseMoved:
			if (menu.play.body.boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
				menu.selected = PLAY;
				menu.update_selected();
			}
			else if (menu.options.body.boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
				menu.selected = OPTIONS;
				menu.update_selected();
			}
			else if (menu.exit.body.boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
				menu.selected = EXIT;
				menu.update_selected();
			}
			break;

		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Button::Left:
				if (menu.selected == PLAY && menu.play.body.boundary.isWithin(Input::translateX(sf::Mouse::getPosition(*_context->window).x), Input::translateY(sf::Mouse::getPosition(*_context->window).y)))
					next();
				else if (menu.selected == EXIT && menu.exit.body.boundary.isWithin(Input::translateX(sf::Mouse::getPosition(*_context->window).x), Input::translateY(sf::Mouse::getPosition(*_context->window).y)))
					_context->window->close();
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

//==================================================================

CharacterSelectState::CharacterSelectState(GFXNet* context) : _context(context) {}

void CharacterSelectState::next()
{
	_context->setState(new LvlSelectState(_context));
}

void CharacterSelectState::prev()
{
	_context->setState(new MainMenuState(_context));
}

void CharacterSelectState::handler()
{
	menu.handler(_context->level->playerMap);
}

void CharacterSelectState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_context->window->close();
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::E:
				_context->level->add_player(-1);
				menu.selectBox[-1].occupied = true;
				break;
			case sf::Keyboard::Q:
				if (_context->level->playerMap.find(-1) != _context->level->playerMap.end()) {
					if (typeid(*_context->level->playerMap[-1]).name() == typeid(Ball).name()) {
						_context->level->playerMap[-1].reset();
						_context->level->playerMap[-1] = unique_ptr<Player>(new Boxy);
					}
					else {
						_context->level->playerMap[-1].reset();
						_context->level->playerMap[-1] = unique_ptr<Player>(new Ball);
					}
				}
				break;
			case sf::Keyboard::Left:
				if (_context->level->playerMap.find(-1) != _context->level->playerMap.end())
					_context->level->playerMap[-1].get()->change_color(PREV);
				break;
			case sf::Keyboard::Right:
				if (_context->level->playerMap.find(-1) != _context->level->playerMap.end())
					_context->level->playerMap[-1].get()->change_color(NEXT);
				break;
			case sf::Keyboard::Return:
				next();
				break;
			case sf::Keyboard::Escape:
				_context->window->close();
				break;
			default:
				break;
			}
			break;
		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case START:
				_context->level->add_player(event.joystickButton.joystickId);
				menu.selectBox[event.joystickButton.joystickId].occupied = true;
				break;
			case X:
				if (_context->level->playerMap.find(event.joystickButton.joystickId) != _context->level->playerMap.end()) {
					if (typeid(*_context->level->playerMap[event.joystickButton.joystickId]).name() == typeid(Ball).name()) {
						_context->level->playerMap[event.joystickButton.joystickId].reset();
						_context->level->playerMap[event.joystickButton.joystickId] = unique_ptr<Player>(new Boxy);
					}
					else {
						_context->level->playerMap[event.joystickButton.joystickId].reset();
						_context->level->playerMap[event.joystickButton.joystickId] = unique_ptr<Player>(new Ball);
					}
				}
				break;
			case B:
				_context->level->playerMap.clear();
				prev();
				break;
			case SELECT:
				next();
				break;
			case LB:
				if (_context->level->playerMap.find(event.joystickButton.joystickId) != _context->level->playerMap.end())
					_context->level->playerMap[event.joystickButton.joystickId].get()->change_color(PREV);
				break;
			case RB:
				if (_context->level->playerMap.find(event.joystickButton.joystickId) != _context->level->playerMap.end())
					_context->level->playerMap[event.joystickButton.joystickId].get()->change_color(NEXT);
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

//==================================================================

LvlSelectState::LvlSelectState(GFXNet* context) : _context(context) {}

void LvlSelectState::next()
{
	_context->setState(new LevelState(_context));
}

void LvlSelectState::prev()
{
	_context->setState(new CharacterSelectState(_context));
}

void LvlSelectState::handler()
{
	menu.handler();
}

void LvlSelectState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_context->window->close();
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
				menu.position++;

				//Prevent underflow
				if (menu.position == UINT_MAX)
					menu.position = 2;

				menu.position = menu.position % 3;

				switch (menu.position)
				{
				case 0:
					menu.selector.center.x = menu.level1.body.center.x;
					menu.selector.center.y = menu.level1.body.center.y;
					break;
				case 1:
					menu.selector.center.x = menu.level2.body.center.x;
					menu.selector.center.y = menu.level2.body.center.y;
					break;
				case 2:
					menu.selector.center.x = menu.level3.body.center.x;
					menu.selector.center.y = menu.level3.body.center.y;
					break;
				}
				break;
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
				menu.position--;

				//Prevent underflow
				if (menu.position == UINT_MAX)
					menu.position = 2;

				menu.position = menu.position % 3;

				switch (menu.position)
				{
				case 0:
					menu.selector.center.x = menu.level1.body.center.x;
					menu.selector.center.y = menu.level1.body.center.y;
					break;
				case 1:
					menu.selector.center.x = menu.level2.body.center.x;
					menu.selector.center.y = menu.level2.body.center.y;
					break;
				case 2:
					menu.selector.center.x = menu.level3.body.center.x;
					menu.selector.center.y = menu.level3.body.center.y;
					break;
				}
				break;
			case sf::Keyboard::Return:
				switch (menu.position)
				{
				case 0:
					_context->level = unique_ptr<Level>(new Field_Level());
					break;
				case 1:
					_context->level = unique_ptr<Level>(new Night_Level());
					break;
				case 2:
					_context->level = unique_ptr<Level>(new Time_Level());
					break;
				}

				_context->level->reset_level();

				next();
				break;
			case sf::Keyboard::Escape:
				_context->window->close();
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case A:
				switch (menu.position)
				{
				case 0:
					_context->level = unique_ptr<Level>(new Field_Level());
					break;
				case 1:
					_context->level = unique_ptr<Level>(new Night_Level());
					break;
				case 2:
					_context->level = unique_ptr<Level>(new Time_Level());
					break;
				}

				_context->level->reset_level();

				next();
				break;
			case B:
				prev();
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickMoved:
		{
			float axis_position1 = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::PovX); //DPAD

			if (axis_position1 == 100) //RIGHT
			{
				menu.position++;
			}
			else if (axis_position1 == -100) //LEFT
			{
				menu.position--;
			}

			//Prevent underflow
			if (menu.position == UINT_MAX)
				menu.position = 2;

			menu.position = menu.position % 3;

			switch (menu.position)
			{
			case 0:
				menu.selector.center.x = menu.level1.body.center.x;
				menu.selector.center.y = menu.level1.body.center.y;
				break;
			case 1:
				menu.selector.center.x = menu.level2.body.center.x;
				menu.selector.center.y = menu.level2.body.center.y;
				break;
			case 2:
				menu.selector.center.x = menu.level3.body.center.x;
				menu.selector.center.y = menu.level3.body.center.y;
				break;
			}
		}
		break;

		default:
			break;
		}
	}
}

//==================================================================

LevelState::LevelState(GFXNet* context) : _context(context) {};

void LevelState::next() { /* Do nothing */ }

void LevelState::prev()
{
	_context->setState(new LvlSelectState(_context));
}

void LevelState::handler()
{
	_context->level->phys_handler();
	_context->level->gfx_handler();
}

void LevelState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_context->window->close();
			break;

		case sf::Event::KeyPressed:
		{
			Player *plyr = NULL;

			if (_context->level->playerMap.find(-1) != _context->level->playerMap.end()) {
				plyr = _context->level->playerMap[-1].get();
				plyr->read_keys(event.key.code);
			}

			switch (event.key.code) {
			case sf::Keyboard::Escape:
				_context->window->close();
				break;
			default:
				break;
			}
		}
			break;

		case sf::Event::KeyReleased:
		{
			Player *plyr = NULL;

			if (_context->level->playerMap.find(-1) != _context->level->playerMap.end()) {
				plyr = _context->level->playerMap[-1].get();
				plyr->read_released_keys(event.key.code);
			}
		}
			break;

		case sf::Event::JoystickButtonPressed:
		{
			Player *plyr = NULL;

			if (_context->level->playerMap.find(event.joystickButton.joystickId) != _context->level->playerMap.end()) {
				plyr = _context->level->playerMap[event.joystickButton.joystickId].get();
				plyr->read_buttons(event.joystickButton.button);
			}
		}
			break;

		case sf::Event::JoystickMoved:
		{
			Player *plyr = NULL;

			if (_context->level->playerMap.find(event.joystickButton.joystickId) != _context->level->playerMap.end()) {
				plyr = _context->level->playerMap[event.joystickButton.joystickId].get();
				plyr->read_axis(event.joystickConnect.joystickId);
			}
		}
			break;

		default:
			break;
		}
	}
}
