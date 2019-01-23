#pragma once

#include "headers.h"
#include "player.h"
#include "menus.h"
#include "state.h"

class RState;

class Command {
public:
	static unique_ptr<Command> create(RState*); //Factory for type of commands
};

/*****************************************************************************************************/
class CommandMenu : public Command {
public:
	virtual void execute(unique_ptr<Menu>& menu) = 0;
	CommandMenu() {}
	virtual ~CommandMenu() {}
};

class MoveLeftCommand : public CommandMenu {
public:
	void execute(unique_ptr<Menu>& menu);
	MoveLeftCommand() {}
	~MoveLeftCommand() {}
};

class MoveRightCommand : public CommandMenu {
public:
	void execute(unique_ptr<Menu>& menu);
	MoveRightCommand() {}
	~MoveRightCommand() {}
};

class MoveUpCommand : public CommandMenu {
public:
	void execute(unique_ptr<Menu>& menu);
	MoveUpCommand() {}
	~MoveUpCommand() {}
};

class MoveDownCommand : public CommandMenu {
public:
	void execute(unique_ptr<Menu>& menu);
	MoveDownCommand() {}
	~MoveDownCommand() {}
};

class ConfirmCommand : public CommandMenu {
public:
	void execute(unique_ptr<Menu>& menu);
	ConfirmCommand() {}
	~ConfirmCommand() {}
};

class DenyCommand : public CommandMenu {
public:
	void execute(unique_ptr<Menu>& menu);
	DenyCommand() {}
	~DenyCommand() {}
};

/*****************************************************************************************************/
class CommandCharSel : public CommandMenu {
public:
	virtual void execute(CharacterSelect& menu, map<unsigned int, unique_ptr<Player>>& players) = 0;
	CommandCharSel() {}
	virtual ~CommandCharSel() {}
};

class AddCharacterCommand : public CommandCharSel {
public:
	void execute(CharacterSelect& menu, map<unsigned int, unique_ptr<Player>>& players);
	AddCharacterCommand() {}
	~AddCharacterCommand() {}
};

class RemoveCharacterCommand : public CommandCharSel {
public:
	void execute(CharacterSelect& menu, map<unsigned int, unique_ptr<Player>>& players);
	RemoveCharacterCommand() {}
	~RemoveCharacterCommand() {}
};

class ChangeCharacterCommand : public CommandCharSel {
public:
	void execute(CharacterSelect& menu, map<unsigned int, unique_ptr<Player>>& players);
	ChangeCharacterCommand() {}
	~ChangeCharacterCommand() {}
};

class NextColorCommand : public CommandCharSel {
public:
	void execute(CharacterSelect& menu, map<unsigned int, unique_ptr<Player>>& players);
	NextColorCommand() {}
	~NextColorCommand() {}
};

class PrevColorCommand : public CommandCharSel {
public:
	void execute(CharacterSelect& menu, map<unsigned int, unique_ptr<Player>>& players);
	PrevColorCommand() {}
	~PrevColorCommand() {}
};

/*****************************************************************************************************/
class CommandPlayer : public Command {
public:
	virtual void execute(Player& player) = 0;
	CommandPlayer() {}
	virtual ~CommandPlayer() {}

	friend class Toggle;
};

class JumpCommand : public CommandPlayer {
public:
	void execute(Player& player);
	JumpCommand() {}
	~JumpCommand() {}

	friend class Toggle;
};

class AttackCommand : public CommandPlayer {
public:
	void execute(Player& player);
	AttackCommand() {}
	~AttackCommand() {}

	friend class Toggle;
};

class LeftCommand : public CommandPlayer {
public:
	void execute(Player& player);
	LeftCommand() {}
	~LeftCommand() {}

	friend class Toggle;
};

class RightCommand : public CommandPlayer {
public:
	void execute(Player& player);
	RightCommand() {}
	~RightCommand() {}

	friend class Toggle;
};