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

class Navigable {
private:
	typedef vector<unique_ptr<geomelt::Shape>> SelectedTextures;
	SelectedTextures textures;
public:
	typedef SelectedTextures::iterator iterator;
	typedef SelectedTextures::const_iterator const_iterator;
	iterator begin() { return textures.begin(); }
	iterator end() { return textures.end(); }

	Navigable() {}
	Navigable(vector<unique_ptr<geomelt::Shape>>& vec);
	~Navigable() {}

	friend class Cursor;
	friend class MainMenu;
	friend class CharacterSelect;
	friend class LevelSelect;
};

class Cursor {
private:
	unique_ptr<Navigable> icons;
	vector<unique_ptr<geomelt::Shape>>::iterator selected;
public:
	void render();
	void updateSelection();

	Cursor() {}
	Cursor(vector<unique_ptr<geomelt::Shape>>& vec);
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
	vector<unique_ptr<geomelt::Shape>> navigable;
	unique_ptr<Cursor> cursor;
public:
	Menu() {}
	~Menu() {}

	friend class MainMenuState;
	friend class CharacterSelectState;
	friend class LvlSelectState;
	friend class PauseState;
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

	friend class PauseState;
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
	Background background;
public:
	void handler();

	LevelSelect();
	~LevelSelect() {}

	friend class LvlSelectState;
};