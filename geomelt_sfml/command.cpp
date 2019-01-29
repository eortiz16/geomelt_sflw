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
	state->next();
}

void DenyCommand::execute(RState* state)
{
	state->prev();
}

/* Specific for all players */
void JumpCommand::execute(RState* state)
{
	state->_context->level->players.jump(this->id);
}

void AttackCommand::execute(RState* state)
{
	state->_context->level->players.attack(this->id);
}

void MoveLeftCommand::execute(RState* state)
{
	state->_context->level->players.move(this->id, LEFT);
}

void MoveRightCommand::execute(RState* state)
{
	state->_context->level->players.move(this->id, RIGHT);
}

void StopCommand::execute(RState * state)
{
	state->_context->level->players.stop(this->id);
}

void ExitCommand::execute(RState* state)
{
	state->_context->window->close();
}

void AddCharacterCommand::execute(RState* state)
{
	state->_context->level->players.add(this->id);
	static_cast<CharacterSelect&>(*static_cast<CharacterSelectState&>(*state).menu).selectBox[id].occupied = true;
}

void RemoveCharacterCommand::execute(RState* state)
{
	state->_context->level->players.purge(this->id);
}

void ChangeCharacterCommand::execute(RState* state)
{
	state->_context->level->players.transform(this->id);
}

void NextColorCommand::execute(RState* state)
{
	state->_context->level->players.change_color(this->id, NEXT);
}

void PrevColorCommand::execute(RState* state)
{
	state->_context->level->players.change_color(this->id, PREV);
}

CommandLevel::CommandLevel(unsigned int id)
{
	this->id = id;
}

void MouseConfirmCommand::execute(RState* state)
{
	if (static_cast<Menu&>(*static_cast<MenuState&>(*state).menu).isWithin(x, y)) 
		state->next();
}

MouseConfirmCommand::MouseConfirmCommand(int x, int y)
{
	this->x = x;
	this->y = y;
}
