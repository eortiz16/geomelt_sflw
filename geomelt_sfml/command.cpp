#include "command.h"

unique_ptr<Command> Command::create(RState* state)
{
	if (typeid(*state) == typeid(MainMenuState) 
		|| typeid(*state) == typeid(LvlSelectState)
		|| typeid(*state) == typeid(PauseState)) {
		return unique_ptr<CommandMenu>();
	}
	else if (typeid(*state) == typeid(CharacterSelectState)) {
		return unique_ptr<CommandCharSel>();
	}
	else if (typeid(*state) == typeid(LevelState)) {
		return unique_ptr<CommandPlayer>();
	}
}

/* Generic for Menu Navigation */
void NavLeftCommand::execute(RState* state)
{
	//state.
}

void NavRightCommand::execute(RState* state)
{
}

void NavUpCommand::execute(RState* state)
{
}

void NavDownCommand::execute(RState* state)
{
}

void ConfirmCommand::execute(RState* state)
{
}

void DenyCommand::execute(RState* state)
{
}

/* Specific to Character Select menu */
void AddCharacterCommand::execute(RState* state)
{
	
}

void RemoveCharacterCommand::execute(RState* state)
{
}

void ChangeCharacterCommand::execute(RState* state)
{
}

void NextColorCommand::execute(RState* state)
{
}

void PrevColorCommand::execute(RState* state)
{
}

/* Specific for all players */
void JumpCommand::execute(RState* state)
{
	//state->_context->level->
	//player.jump();
}

void AttackCommand::execute(RState* state)
{
	//player.toggle.attacking ^= 1;
	//player.toggle.attackTimer.restart();
}

void MoveLeftCommand::execute(RState* state)
{
	//player.toggle.walking = true;
	//player.direction = LEFT;
}

void MoveRightCommand::execute(RState* state)
{
	//player.toggle.walking = true;
	//player.direction = RIGHT;
}


