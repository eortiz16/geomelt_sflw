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

	return unique_ptr<Command>();
}

/* Generic for Menu Navigation */
void MoveLeftCommand::execute(unique_ptr<Menu>& menu)
{
}

void MoveRightCommand::execute(unique_ptr<Menu>& menu)
{
}

void MoveUpCommand::execute(unique_ptr<Menu>& menu)
{
}

void MoveDownCommand::execute(unique_ptr<Menu>& menu)
{
}

void ConfirmCommand::execute(unique_ptr<Menu>& menu)
{
}

void DenyCommand::execute(unique_ptr<Menu>& menu)
{
}

/* Specific to Character Select screen */
void AddCharacterCommand::execute(CharacterSelect & menu, map<unsigned int, unique_ptr<Player>>& players)
{
}

void RemoveCharacterCommand::execute(CharacterSelect & menu, map<unsigned int, unique_ptr<Player>>& players)
{
}

void ChangeCharacterCommand::execute(CharacterSelect & menu, map<unsigned int, unique_ptr<Player>>& players)
{
}

void NextColorCommand::execute(CharacterSelect & menu, map<unsigned int, unique_ptr<Player>>& players)
{
}

void PrevColorCommand::execute(CharacterSelect & menu, map<unsigned int, unique_ptr<Player>>& players)
{
}

/* Specific for all players */
void JumpCommand::execute(Player & player)
{
	player.jump();
}

void AttackCommand::execute(Player & player)
{
	player.toggle.attacking ^= 1;
	player.toggle.attackTimer.restart();
}

void LeftCommand::execute(Player & player)
{
	player.toggle.walking = true;
	player.direction = LEFT;
}

void RightCommand::execute(Player & player)
{
	player.toggle.walking = true;
	player.direction = RIGHT;
}


