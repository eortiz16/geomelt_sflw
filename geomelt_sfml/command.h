#pragma once

#include "headers.h"
#include "player.h"
#include "menus.h"
#include "state.h"

/*
I may need to abstract this a bit further. For example, A LevelCommand should be added.
This class should have a vector of player commands with controller_id as a private member.
This will allow for the identification of player input.
*/

class RState;

class Command {
public:
	virtual void execute(RState* state) = 0;
	static unique_ptr<Command> create(RState*); //Factory for type of commands
};

class ExitCommand : public Command {
public:
	void execute(RState* state);
	ExitCommand() {}
	virtual ~ExitCommand() {}
};

class ConfirmCommand : public Command {
private:
	int x;
	int y;
public:
	void execute(RState* state);
	ConfirmCommand() {}
	ConfirmCommand(int x, int y);
	~ConfirmCommand() {}
};

class DenyCommand : public Command {
public:
	void execute(RState* state);
	DenyCommand() {}
	~DenyCommand() {}
};

/*****************************************************************************************************/
class CommandMenu : public Command {
public:
	virtual void execute(RState* state) = 0;
	CommandMenu() {}
	virtual ~CommandMenu() {}
};

class NextCommand : public CommandMenu {
public:
	void execute(RState* state);
	NextCommand() {}
	~NextCommand() {}
};

class PrevCommand : public CommandMenu {
public:
	void execute(RState* state);
	PrevCommand() {}
	~PrevCommand() {}
};

/*****************************************************************************************************/
class CommandPlayer;

class CommandLevel : public Command {
private:
	unsigned int id;
public:
	virtual void execute(RState * state) = 0;
	CommandLevel() {}
	CommandLevel(unsigned int);
	virtual ~CommandLevel() {}

	friend class Toggle;
	friend class LevelState;
	friend class CharacterSelectState;
	friend class CommandPlayer;
	friend class CommandCharSel;
	friend class JumpCommand;
	friend class AttackCommand;
	friend class MoveLeftCommand;
	friend class MoveRightCommand;
	friend class StopCommand;
	friend class AddCharacterCommand;
	friend class RemoveCharacterCommand;
	friend class ChangeCharacterCommand;
	friend class NextColorCommand;
	friend class PrevColorCommand;
	friend class LevelState;
	friend class CharacterSelectState;
};

class CommandPlayer : public CommandLevel {
public:
	virtual void execute(RState * state) = 0;
	CommandPlayer() {}
	CommandPlayer(unsigned int id) : CommandLevel(id) {};
	virtual ~CommandPlayer() {}

	friend class Toggle;
	friend class LevelState;
};

class CommandCharSel : public CommandLevel {
public:
	virtual void execute(RState * state) = 0;
	CommandCharSel() {}
	CommandCharSel(unsigned int id) : CommandLevel(id) {};
	virtual ~CommandCharSel() {}

	friend class Toggle;
	friend class CharacterSelectState;
};

/*****************************************************************************************************/
class JumpCommand : public CommandPlayer {
public:
	void execute(RState* state);
	JumpCommand() {}
	JumpCommand(unsigned int id) : CommandPlayer(id) {};
	~JumpCommand() {}

	friend class Toggle;
};

class AttackCommand : public CommandPlayer {
public:
	void execute(RState* state);
	AttackCommand() {}
	AttackCommand(unsigned int id) : CommandPlayer(id) {};
	~AttackCommand() {}

	friend class Toggle;
};

class MoveLeftCommand : public CommandPlayer {
public:
	void execute(RState* state);
	MoveLeftCommand() {}
	MoveLeftCommand(unsigned int id) : CommandPlayer(id) {};
	~MoveLeftCommand() {}

	friend class Toggle;
};

class MoveRightCommand : public CommandPlayer {
public:
	void execute(RState* state);
	MoveRightCommand() {}
	MoveRightCommand(unsigned int id) : CommandPlayer(id) {};
	~MoveRightCommand() {}

	friend class Toggle;
};

class StopCommand : public CommandPlayer {
public:
	void execute(RState* state);
	StopCommand() {}
	StopCommand(unsigned int id) : CommandPlayer(id) {};
	~StopCommand() {}

	friend class Toggle;
};

class AddCharacterCommand : public CommandCharSel {
public:
	void execute(RState* state);
	AddCharacterCommand() {}
	AddCharacterCommand(unsigned int id) : CommandCharSel(id) {};
	~AddCharacterCommand() {}
};

class RemoveCharacterCommand : public CommandCharSel {
public:
	void execute(RState* state);
	RemoveCharacterCommand() {}
	RemoveCharacterCommand(unsigned int id) : CommandCharSel(id) {};
	~RemoveCharacterCommand() {}
};

class ChangeCharacterCommand : public CommandCharSel {
public:
	void execute(RState* state);
	ChangeCharacterCommand() {}
	ChangeCharacterCommand(unsigned int id) : CommandCharSel(id) {};
	~ChangeCharacterCommand() {}
};

class NextColorCommand : public CommandCharSel {
public:
	void execute(RState* state);
	NextColorCommand() {}
	NextColorCommand(unsigned int id) : CommandCharSel(id) {};
	~NextColorCommand() {}
};

class PrevColorCommand : public CommandCharSel {
public:
	void execute(RState* state);
	PrevColorCommand() {}
	PrevColorCommand(unsigned int id) : CommandCharSel(id) {};
	~PrevColorCommand() {}
};
