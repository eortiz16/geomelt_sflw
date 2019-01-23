#pragma once

#include "headers.h"
#include "assets.h"
#include "complex_shapes.h"
#include "controller.h"

constexpr auto SELECTED_CARDINALITY = 3;
enum Selected { PLAY, OPTIONS, EXIT };

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

class Cursor {
private:
	vector<TexturedQuad> selectedTextures;
	unsigned int selected;
public:
	void render();
	Cursor() {}
	Cursor(vector<TexturedQuad>& vec);
	void updateSelection();
	~Cursor() {}

	friend class Menu;
	friend class MainMenu;
	friend class LevelSelect;
	friend class Pause;
	friend class MainMenuState;
	friend class CharacterSelectState;
	friend class LvlSelectState;
	friend class PauseState;
};

class Menu {
protected:
	vector<TexturedQuad> navigable;
	unique_ptr<Cursor> cursor;
public:
	Menu() {}
	~Menu() {}
};

class MainMenu : public Menu {
private:
	TexturedQuad title;
	sf::Text text;
public:
	void handler(unique_ptr<Level>& level);

	MainMenu();
	~MainMenu() {}

	friend class MainMenuState;
};

class Pause : public Menu {
public:
	void handler(unique_ptr<Level>& level);

	Pause() {}
	~Pause() {}
};

class CharacterSelect : public Menu {
private:
	Background background;
	vector<CharSelBox> selectBox;
public:
	void handler(map<unsigned int, unique_ptr<Player>>& players);

	CharacterSelect();
	~CharacterSelect() {}

	friend class CharacterSelectState;
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
	void handler();

	LevelSelect();
	~LevelSelect() {}

	friend class LvlSelectState;
};