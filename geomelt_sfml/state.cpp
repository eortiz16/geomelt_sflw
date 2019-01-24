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
		//command->create(_state);

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

MainMenuState::MainMenuState(GFXNet* context) 
{ 
	this->_context = context; 
	menu = unique_ptr<Menu>(new MainMenu);
}

void MainMenuState::next()
{
	_context->setState(new CharacterSelectState(_context));
	_context->command = Command::create(_context->_state);
}

void MainMenuState::prev() { /* Do nothing */ }

void MainMenuState::handler()
{
	static_cast<MainMenu&>(*menu).handler(_context->level);
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
				menu->cursor->selected--;
				menu->cursor->updateSelection();
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				menu->cursor->selected++;
				menu->cursor->updateSelection();
				break;
			case sf::Keyboard::Return:
				if (menu->cursor->selected == menu->cursor->icons->begin())
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
				if (menu->cursor->selected == menu->cursor->icons->begin())
					next();
				if (menu->cursor->selected == menu->cursor->icons->begin() + EXIT)
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
				menu->cursor->selected--;
			else if (axis_position1 == -100) //down
				menu->cursor->selected++;

			menu->cursor->updateSelection();
		}
			break;
		case sf::Event::MouseMoved:
			if (menu->navigable[PLAY]->boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
				menu->cursor->selected = menu->cursor->icons->begin();
			}
			else if (menu->navigable[OPTIONS]->boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
				menu->cursor->selected = menu->cursor->icons->begin() + OPTIONS;
			}
			else if (menu->navigable[EXIT]->boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
				menu->cursor->selected = menu->cursor->icons->begin() + EXIT;
			}
			break;

		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Button::Left:
				if (menu->cursor->selected == menu->cursor->icons->begin() && menu->navigable[PLAY]->boundary.isWithin(Input::translateX(sf::Mouse::getPosition(*_context->window).x), Input::translateY(sf::Mouse::getPosition(*_context->window).y)))
					next();
				else if (menu->cursor->selected == menu->cursor->icons->begin() + EXIT && menu->navigable[EXIT]->boundary.isWithin(Input::translateX(sf::Mouse::getPosition(*_context->window).x), Input::translateY(sf::Mouse::getPosition(*_context->window).y)))
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

CharacterSelectState::CharacterSelectState(GFXNet* context)
{
	this->_context = context;
	menu.release();
	menu = unique_ptr<Menu>(new CharacterSelect);
}


void CharacterSelectState::next()
{
	_context->setState(new LvlSelectState(_context));
	_context->command = Command::create(_context->_state);
}

void CharacterSelectState::prev()
{
	_context->setState(new MainMenuState(_context));
	_context->command = Command::create(_context->_state);
}

void CharacterSelectState::handler()
{
	static_cast<CharacterSelect&>(*menu).handler(_context->level->playerMap);
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
				static_cast<CharacterSelect&>(*menu).selectBox[-1].occupied = true;
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
				static_cast<CharacterSelect&>(*menu).selectBox[event.joystickButton.joystickId].occupied = true;
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

LvlSelectState::LvlSelectState(GFXNet* context)
{
	this->_context = context;
	menu.release();
	menu = unique_ptr<Menu>(new LevelSelect);
}

void LvlSelectState::next()
{
	_context->setState(new LevelState(_context));
	_context->command = Command::create(_context->_state);
}

void LvlSelectState::prev()
{
	_context->setState(new CharacterSelectState(_context));
	_context->command = Command::create(_context->_state);
}

void LvlSelectState::handler()
{
	static_cast<LevelSelect&>(*menu).handler();
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
				menu->cursor->selected++;
				menu->cursor->updateSelection();
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
				menu->cursor->selected--;
				menu->cursor->updateSelection();
				break;
			case sf::Keyboard::Return:
				_context->level = Level::make((Lvl)(std::distance( menu->cursor->icons->begin(), menu->cursor->selected)));
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
				_context->level = Level::make((Lvl)(std::distance(menu->cursor->icons->begin(), menu->cursor->selected)));
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
				menu->cursor->selected++;
				menu->cursor->updateSelection();
			}
			else if (axis_position1 == -100) //LEFT
			{
				menu->cursor->selected--;
				menu->cursor->updateSelection();
			}
		}
		break;

		default:
			break;
		}
	}
}

//==================================================================

LevelState::LevelState(GFXNet* context) 
{ 
	this->_context = context; 
}

void LevelState::next() 
{ 
	_context->setState(new PauseState(_context));
	_context->command = Command::create(_context->_state);
}

void LevelState::prev()
{
	_context->setState(new LvlSelectState(_context));
	_context->command = Command::create(_context->_state);
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

			switch (event.joystickButton.button) {
			case START:
				next();
				break;
			default:
				break;
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

//==================================================================

PauseState::PauseState(GFXNet* context) 
{
	menu.release();
	menu = unique_ptr<Menu>(new Pause);
	this->_context = context; 
}

void PauseState::next() { /* Do nothing */ }

void PauseState::prev()
{
	_context->setState(new LevelState(_context));
	_context->command = Command::create(_context->_state);
}

void PauseState::handler()
{
	_context->level->gfx_handler();
}

void PauseState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_context->window->close();
			break;

		case sf::Event::JoystickButtonPressed:
		{
			switch (event.joystickButton.button) {
			case START:
			case B:
				prev();
				break;
			default:
				break;
			}
		}
		break;

		default:
			break;
		}
	}
}
