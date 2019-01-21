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

//*** REDESIGN: FAVOR COMPOSITION OVER INHERITANCE ***/
class Menu {
public:
	//BUTTONS
	virtual int read_buttons(sf::Event event, CurrentGameState &current) = 0;
	virtual void read_buttons(sf::Event event, CurrentGameState &current, unique_ptr<Level> &level) = 0;

	virtual void read_axis(unsigned int joyID) = 0;

	virtual void handler(map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin) = 0;
	virtual void handler() = 0;
};

class MainMenu : public Menu {
private:
	Selected selected;
	TexturedQuad title;
	TexturedQuad play;
	TexturedQuad options;
	TexturedQuad exit;
	TexturedQuad selectedIcon;
	unique_ptr<Level> level;
	sf::Text text;
public:
	int read_buttons(sf::Event event, CurrentGameState &current);
	void read_buttons(sf::Event event, CurrentGameState &current, unique_ptr<Level> &window) {}
	void read_axis(unsigned int joyID);
	void handler();
	void handler(map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin) {}

	MainMenu();
	~MainMenu();
};

class Pause : public Menu {
public:
	void read_axis(unsigned int joyID) {}

	//void handler(Game *game) {}
};

class CharacterSelect : public Menu {
private:
	Background background;
	vector<CharSelBox> selectBox;
public:
	int read_buttons(sf::Event event, CurrentGameState &current) { return -1;  }
	void read_buttons(sf::Event event, CurrentGameState &current, unique_ptr<Level> &level);
	void read_axis(unsigned int joyID) {}	
	void handler() {}
	void handler(map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin);

	CharacterSelect();
};

class LevelSelect : public Menu {
private:
	unsigned int position;
	geomelt::Quad selector;
	Background background;
	TexturedQuad level1;
	TexturedQuad level2;
	TexturedQuad level3;
public:
	int read_buttons(sf::Event event, CurrentGameState &current) { return -1; }
	void read_buttons(sf::Event event, CurrentGameState &current, unique_ptr<Level> &level);
	void read_axis(unsigned int joyID);
	void handler();
	void handler(map<unsigned int, unique_ptr<Player>>::iterator it, map<unsigned int, unique_ptr<Player>>::iterator fin) {}

	LevelSelect();
};