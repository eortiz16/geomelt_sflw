#include "command.h"

/* This will probably only respond to keys and buttons */

unique_ptr<Command> Command::create(RState* state)
{
	if (typeid(*state) == typeid(MainMenuState) 
		|| typeid(*state) == typeid(LevelSelectState)
		|| typeid(*state) == typeid(PauseState)) {
		return unique_ptr<CommandMenu>();
	}
	else if (typeid(*state) == typeid(CharacterSelectState)
		|| typeid(*state) == typeid(LevelState)) {
		return unique_ptr<CommandLevel>();
	}

	return unique_ptr<Command>(); 
}

/* Generic for Menu Navigation */
void PrevCommand::execute(RState* state)
{
	static_cast<MenuState&>(*state).menu->cursor->selected--;
	static_cast<MenuState&>(*state).menu->cursor->updateSelection();
}

void NextCommand::execute(RState* state)
{
	static_cast<MenuState&>(*state).menu->cursor->selected++;
	static_cast<MenuState&>(*state).menu->cursor->updateSelection();
}

void ConfirmCommand::execute(RState* state)
{
	if (typeid(state).name() != typeid(MainMenuState).name())
		state->next();
	else if (static_cast<MainMenu&>(*static_cast<MainMenuState&>(*state).menu).isWithin(x,y))
		state->next();
}

ConfirmCommand::ConfirmCommand(int x, int y)
{
	this->x = x;
	this->y = y;
}

void DenyCommand::execute(RState* state)
{
	state->prev();
}

/* Specific for all players */
void JumpCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end())
		state->_context->level->playerMap[id]->jump();
}

void AttackCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end()) {
		state->_context->level->playerMap[id]->toggle.attacking ^= 1;
		state->_context->level->playerMap[id]->toggle.attackTimer.restart();
	}
}

void MoveLeftCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end()) {
		state->_context->level->playerMap[id]->toggle.walking = true;
		state->_context->level->playerMap[id]->direction = LEFT;
	}
}

void MoveRightCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end()) {
		state->_context->level->playerMap[id]->toggle.walking = true;
		state->_context->level->playerMap[id]->direction = RIGHT;
	}
}

void StopCommand::execute(RState * state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end()) {
		state->_context->level->playerMap[id]->toggle.walking = false;
	}
}

void ExitCommand::execute(RState* state)
{
	state->_context->window->close();
}

void AddCharacterCommand::execute(RState* state)
{
	state->_context->level->add_player(id);
	static_cast<CharacterSelect&>(*static_cast<CharacterSelectState&>(*state).menu).selectBox[id].occupied = true;
}

void RemoveCharacterCommand::execute(RState* state)
{
}

void ChangeCharacterCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end()) {
		if (typeid(*state->_context->level->playerMap[id]).name() == typeid(Ball).name()) {
			state->_context->level->playerMap[id].reset();
			state->_context->level->playerMap[id] = unique_ptr<Player>(new Boxy);
		}
		else {
			state->_context->level->playerMap[id].reset();
			state->_context->level->playerMap[id] = unique_ptr<Player>(new Ball);
		}
	}
}

void NextColorCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end())
		state->_context->level->playerMap[id].get()->change_color(NEXT);
}

void PrevColorCommand::execute(RState* state)
{
	if (state->_context->level->playerMap.find(id) != state->_context->level->playerMap.end())
		state->_context->level->playerMap[id].get()->change_color(PREV);
}

CommandLevel::CommandLevel(unsigned int id)
{
	this->id = id;
}
