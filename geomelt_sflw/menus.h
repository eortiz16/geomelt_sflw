#pragma once

#include "headers.h"
#include "assets.h"
#include "complex_shapes.h"
#include "controller.h"

constexpr auto SELECTED_CARDINALITY = 3;
enum Selected { PLAY, OPTIONS, EXIT };
enum CurrentMenu {MAINMENU, CHARSEL, LEVELSEL, NONE};

inline void operator++(Selected &sel, int)
{
	const int i = static_cast<int>(sel);
	sel = static_cast<Selected>((i + 1) % SELECTED_CARDINALITY);
}

inline void operator--(Selected &sel, int)
{
	const int i = static_cast<int>(sel);
	sel = static_cast<Selected>((i - 1) % SELECTED_CARDINALITY);
	if (sel < 0)
		sel = static_cast<Selected> (SELECTED_CARDINALITY - 1);
}

class CurrentGameState
{
private:
	Render_State render;
	CurrentMenu menu;
	bool menuChange;

	friend class CharacterSelect;
	friend class MainMenu;
	friend class LevelSelect;
	friend class Game;
	friend class Input;
};

class Menu {
public:
	//BUTTONS
	virtual void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level) = 0;
	virtual void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<sf::RenderWindow> &window) = 0;

	virtual void read_axis(unsigned int joyID, Assets assets) = 0;
	virtual void handler(Camera camera, Assets assets, map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin) = 0;
	virtual void handler() = 0;
	Menu() {}
};

class MainMenu : public Menu {
private:
	Selected selected;
	TexturedQuad title;
	TexturedQuad play;
	TexturedQuad options;
	TexturedQuad exit;
	TexturedQuad selectedIcon;
	sf::Text text;
public:
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level) {}
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<sf::RenderWindow> &window);
	void read_axis(unsigned int joyID, Assets assets);
	void handler();
	void handler(Camera camera, Assets assets, map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin) {}

	MainMenu() {}
	MainMenu(Assets assets);
	~MainMenu();
};

class Pause : public Menu {
public:
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level) {}
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<sf::RenderWindow> &window) {}

	void read_axis(unsigned int joyID, Assets assets) {}

	//void handler(Game *game) {}
	Pause() {}
};

class CharacterSelect : public Menu {
private:
	Background background;
	CharSelBox selectBox[4];
public:

	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level);
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<sf::RenderWindow> &window) {}
	void read_axis(unsigned int joyID, Assets assets) {}	
	void handler() {}
	void handler(Camera camera, Assets assets, map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin);

	CharacterSelect() {}
	CharacterSelect(Assets assets);
};

class LevelSelect : public Menu {
private:
	unsigned int position;
	medmelt::Quad selector;
	Background background;
	TexturedQuad level1;
	TexturedQuad level2;
	TexturedQuad level3;
public:
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<sf::RenderWindow> &window) {}
	void read_buttons(sf::Event event, Assets assets, CurrentGameState &current, unique_ptr<Level> &level);
	void read_axis(unsigned int joyID, Assets assets);
	void handler();
	void handler(Camera camera, Assets assets, map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin) {}

	LevelSelect() {}
	LevelSelect(Assets assets);
};