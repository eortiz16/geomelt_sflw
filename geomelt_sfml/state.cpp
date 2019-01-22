#include "state.h"

GFXNet::GFXNet()
{
	srand((unsigned int)time(NULL));

	// Request a 24-bits depth buffer when creating the window
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;

	// Default to Main Menu
	_state = new MainMenuState(this);

	// To draw the back ground
	level = (rand() % 2 == 0) ? unique_ptr<Level>(new Field_Level())
		: unique_ptr<Level>(new Night_Level());

	window = unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(HDX, HDY), "Geometric Meltdown", sf::Style::Default, contextSettings));
	//window = unique_ptr<RenderWindow> (new sf::RenderWindow(sf::VideoMode(HDX, HDY), "Geometric Meltdown", sf::Style::Fullscreen, contextSettings));

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

		_state->read_buttons();
		_state->read_axis();

		while (sync.lag >= MS_PER_UPDATE())
		{
			if (typeid(_state) == typeid(LevelState))
				level->phys_handler();
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

void MainMenuState::read_buttons()
{
	bool mod = false;
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
			case sf::Keyboard::Enter:
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
		default:
			break;
		}
	}
}

void MainMenuState::read_axis()
{

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

void CharacterSelectState::read_buttons()
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
			cout << "!";
			switch (event.key.code) {
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				break;
			case sf::Keyboard::Enter:
				next();
				break;
			case sf::Keyboard::Escape:
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

void CharacterSelectState::read_axis()
{

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

void LvlSelectState::read_buttons()
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
			cout << "!";
			switch (event.key.code) {
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				break;
			case sf::Keyboard::Enter:
				next();
				break;
			case sf::Keyboard::Escape:
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

void LvlSelectState::read_axis()
{

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

void LevelState::read_buttons()
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
			cout << "!";
			switch (event.key.code) {
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				break;
			case sf::Keyboard::Enter:
				next();
				break;
			case sf::Keyboard::Escape:
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

void LevelState::read_axis()
{

}