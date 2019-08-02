#include "state.h"

/* Game class abstracted as a network of states */
int main()
{
	GFXNet geomelt;
	geomelt.loop();

	return EXIT_SUCCESS;
}

/* Utilize the State Design Pattern to control the transition of menus and levels. */

GFXNet::GFXNet()
{
	srand((unsigned int)time(NULL));

	// Request a 24-bits depth buffer when creating the window
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;
	contextSettings.antialiasingLevel = 16; //MAX

	// Default to Main Menu
	_state = new MainMenuState(this);
	level = unique_ptr<Level>(new Level);

	window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(SCRN_WD, SCRN_HT), "Geometric Meltdown", sf::Style::Default, contextSettings));
	//window = unique_ptr<sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode(SCRN_WD, SCRN_HT), "Geometric Meltdown", sf::Style::Fullscreen, contextSettings));

	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(FPS);
	window->setActive(true);// Active for OPENGL
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

	sync.previous = sync.gameClock.getElapsedTime().asMilliseconds();
}

/* Main Game Loop */
void GFXNet::loop()
{
	while (window->isOpen())
	{
		sync.update();
		_state->read_input();
		
		//Input via Command, once executed release
		if (command) {
			command->execute(_state);
			command.release();
		}

		sync.catch_up();
		window->setActive(true);
		_state->handler(); //Render

		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);// Transformations
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
	_context = context; 
	menu = unique_ptr<Menu>(new MainMenu);
}

void MainMenuState::next()
{
	if (this->menu->cursor->selected == this->menu->cursor->icons->begin() + PLAY) {
		_context->setState(new CharacterSelectState(_context));
		_context->command = Command::create(_context->_state);
	}
	else if (this->menu->cursor->selected == this->menu->cursor->icons->begin() + EXIT) {
		this->_context->window->close();
	}
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
			_context->command = unique_ptr<Command>(new ExitCommand);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				_context->command = unique_ptr<CommandMenu>(new PrevCommand);
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				this->_context->command = unique_ptr<CommandMenu>(new NextCommand);
				break;
			case sf::Keyboard::Return:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			case sf::Keyboard::Escape:
				_context->command = unique_ptr<Command>(new ExitCommand);
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case xbox::A:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickMoved:
		{
			float axis_position1 = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::PovY); //DPAD

			if (axis_position1 == 100) 
				_context->command = unique_ptr<CommandMenu>(new PrevCommand);
			else if (axis_position1 == -100) 
				_context->command = unique_ptr<CommandMenu>(new NextCommand);
		}
			break;

		case sf::Event::MouseMoved:
			for (vector<unique_ptr<Shape>>::iterator it = menu->navigable.begin(); it != menu->navigable.end(); ++it) {
				int index = it - menu->navigable.begin();

				if (it->get()->boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y)))
					menu->cursor->selected = menu->cursor->icons->begin() + index;
			}

			break;

		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Button::Left:
				_context->command = unique_ptr<Command>(new MouseConfirmCommand(sf::Mouse::getPosition(*_context->window).x, sf::Mouse::getPosition(*_context->window).y));
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
	_context->setState(new LevelSelectState(_context));
	_context->command = Command::create(_context->_state);
}

void CharacterSelectState::prev()
{
	_context->level->_players.clear();
	_context->setState(new MainMenuState(_context));
	_context->command = Command::create(_context->_state);
}

void CharacterSelectState::handler()
{
	static_cast<CharacterSelect&>(*menu).handler(_context->level->_players);
}

void CharacterSelectState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_context->command = unique_ptr<Command>(new ExitCommand);
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::E:
				_context->command = unique_ptr<Command>(new AddCharacterCommand(KEY_ID));
				break;
			case sf::Keyboard::Q:
				_context->command = unique_ptr<Command>(new ChangeCharacterCommand(KEY_ID));
				break;
			case sf::Keyboard::Left:
				_context->command = unique_ptr<Command>(new PrevColorCommand(KEY_ID));
				break;
			case sf::Keyboard::Right:
				_context->command = unique_ptr<Command>(new NextColorCommand(KEY_ID));
				break;
			case sf::Keyboard::Return:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			case sf::Keyboard::Escape:
				_context->command = unique_ptr<Command>(new ExitCommand);
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case xbox::START:
				_context->command = unique_ptr<Command>(new AddCharacterCommand(event.joystickButton.joystickId));
				break;
			case xbox::X:
				_context->command = unique_ptr<Command>(new ChangeCharacterCommand(event.joystickButton.joystickId));
				break;
			case xbox::B:
				if (_context->level->_players.size() == 0)
					_context->command = unique_ptr<Command>(new DenyCommand);
				else
					_context->command = unique_ptr<Command>(new RemoveCharacterCommand(event.joystickButton.joystickId));
				break;
			case xbox::SELECT:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			case xbox::LB:
				_context->command = unique_ptr<Command>(new PrevColorCommand(event.joystickButton.joystickId));
				break;
			case xbox::RB:
				_context->command = unique_ptr<Command>(new NextColorCommand(event.joystickButton.joystickId));
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

LevelSelectState::LevelSelectState(GFXNet* context)
{
	this->_context = context;
	menu.release();
	menu = unique_ptr<Menu>(new LevelSelect);
}

void LevelSelectState::next()
{
	_context->level->_scenery = move(SceneryGroup::create((LevelType)(distance(menu->cursor->icons->begin(), menu->cursor->selected))));
	_context->level->_players.reset();
	_context->setState(new LevelState(_context));
	_context->command = Command::create(_context->_state);
}

void LevelSelectState::prev()
{
	_context->setState(new CharacterSelectState(_context));
	_context->command = Command::create(_context->_state);
}

void LevelSelectState::handler()
{
	static_cast<LevelSelect&>(*menu).handler();
}

void LevelSelectState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)	{
		case sf::Event::Closed:
			_context->window->close();
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
				_context->command = unique_ptr<CommandMenu>(new PrevCommand);
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
				_context->command = unique_ptr<CommandMenu>(new NextCommand);
				break;
			case sf::Keyboard::Return:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			case sf::Keyboard::Escape:
				_context->command = unique_ptr<Command>(new ExitCommand);				
				break;
			default:
				break;
			}
			break;

		case sf::Event::MouseMoved:
			for (vector<unique_ptr<Shape>>::iterator it = menu->navigable.begin(); it != menu->navigable.end(); ++it) {
				int index = it - menu->navigable.begin();

				if (it->get()->boundary.isWithin(Input::translateX(event.mouseMove.x), Input::translateY(event.mouseMove.y))) {
					menu->cursor->selected = menu->cursor->icons->begin() + index;
				}
			}

			break;

		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Button::Left:
				_context->command = unique_ptr<Command>(new MouseConfirmCommand(sf::Mouse::getPosition(*_context->window).x, sf::Mouse::getPosition(*_context->window).y));
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case xbox::A:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			case xbox::B:
				_context->command = unique_ptr<Command>(new DenyCommand);				
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickMoved:
		{
			float axis_position1 = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::PovX); //DPAD

			if (axis_position1 == 100) //RIGHT
				_context->command = unique_ptr<CommandMenu>(new NextCommand);
			else if (axis_position1 == -100) //LEFT
				_context->command = unique_ptr<CommandMenu>(new PrevCommand);
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
	this->_context->command = Command::create(this);
	this->_context->level->_overlay = Overlay(this->_context->level->_players);
}

void LevelState::next()
{ 
	_context->setState(new PauseState(_context));
	_context->command = Command::create(_context->_state);
}

void LevelState::prev()
{
	_context->setState(new LevelSelectState(_context));
	_context->command = Command::create(_context->_state);
}

void LevelState::handler()
{
	_context->level->phys_handler();
	_context->level->gfx_handler();
	_context->level->event_handler();
}

void LevelState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			_context->command = unique_ptr<Command>(new ExitCommand);
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				_context->command = unique_ptr<CommandPlayer>(new MoveLeftCommand(KEY_ID));
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				_context->command = unique_ptr<CommandPlayer>(new MoveRightCommand(KEY_ID));
				break;
			case sf::Keyboard::Space:
				_context->command = unique_ptr<CommandPlayer>(new JumpCommand(KEY_ID));
				break;
			case sf::Keyboard::E:
				_context->command = unique_ptr<CommandPlayer>(new AttackCommand(KEY_ID));
				break;
			case sf::Keyboard::Escape:
				_context->command = unique_ptr<Command>(new ExitCommand);
				break;
			default:
				break;
			}
			break;

		case sf::Event::KeyReleased:
			switch (event.key.code) {
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				_context->command = unique_ptr<CommandPlayer>(new StopCommand(KEY_ID));
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case xbox::A:
			case xbox::B:
				_context->command = unique_ptr<CommandPlayer>(new AttackCommand(event.joystickButton.joystickId));
				break;
			case xbox::X:
			case xbox::Y:
				_context->command = unique_ptr<CommandPlayer>(new JumpCommand(event.joystickButton.joystickId));
				break;
			case xbox::START:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickMoved:
		{
			float axis_position = 0.0f;

			switch (event.joystickMove.axis) {
			case sf::Joystick::PovX:
				axis_position = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::PovX);

				if (axis_position == 100.0)
					_context->command = unique_ptr<CommandPlayer>(new MoveRightCommand(event.joystickMove.joystickId));
				else if (axis_position == -100.0)
					_context->command = unique_ptr<CommandPlayer>(new MoveLeftCommand(event.joystickMove.joystickId));
				else if (axis_position == 0.0f)
					_context->command = unique_ptr<CommandPlayer>(new StopCommand(event.joystickMove.joystickId));
				break;

			case sf::Joystick::X:
				axis_position = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::X);

				if (axis_position == 100.0)
					_context->command = unique_ptr<CommandPlayer>(new MoveRightCommand(event.joystickMove.joystickId));
				else if (axis_position == -100.0)
					_context->command = unique_ptr<CommandPlayer>(new MoveLeftCommand(event.joystickMove.joystickId));
				else 
					_context->command = unique_ptr<CommandPlayer>(new StopCommand(event.joystickMove.joystickId));
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

//==================================================================

PauseState::PauseState(GFXNet* context) 
{
	this->_context = context;
	menu.release();
	menu = unique_ptr<Menu>(new Pause);
}

void PauseState::next() 
{ 
	if (this->menu->cursor->selected == this->menu->cursor->icons->begin() + 0) {
		prev();
	}
	else if (this->menu->cursor->selected == this->menu->cursor->icons->begin() + 1) {
		this->_context->window->close();
	}
}

void PauseState::prev()
{
	_context->setState(new LevelState(_context));
	_context->command = Command::create(_context->_state);
}

void PauseState::handler()
{
	_context->level->gfx_handler();
	static_cast<Pause&>(*menu).handler();
}

void PauseState::read_input()
{
	sf::Event event;

	while (_context->window->pollEvent(event))
	{
		switch (event.type)	{
		case sf::Event::Closed:
			_context->window->close();
			break;

		case sf::Event::JoystickButtonPressed:
			switch (event.joystickButton.button) {
			case xbox::START:
			case xbox::B:
				_context->command = unique_ptr<Command>(new DenyCommand);
				break;
			case xbox::A:
				_context->command = unique_ptr<Command>(new ConfirmCommand);
				break;
			default:
				break;
			}
			break;

		case sf::Event::JoystickMoved:
		{
			float axis_position1 = sf::Joystick::getAxisPosition(event.joystickMove.joystickId, sf::Joystick::PovY); //DPAD

			if (axis_position1 == 100)
				_context->command = unique_ptr<CommandMenu>(new PrevCommand);
			else if (axis_position1 == -100)
				_context->command = unique_ptr<CommandMenu>(new NextCommand);
		}
		break;

		default:
			break;
		}
	}
}

