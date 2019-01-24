#pragma once

#include "headers.h"
#include "player.h"
#include "menus.h"
#include "state.h"

class RState;

class Command {
public:
	virtual void execute(RState* state) = 0;
	static unique_ptr<Command> create(RState*); //Factory for type of commands
};

/*****************************************************************************************************/
class CommandMenu : public Command {
public:
	virtual void execute(RState* state) = 0;
	CommandMenu() {}
	virtual ~CommandMenu() {}
};

class NavLeftCommand : public CommandMenu {
public:
	void execute(RState* state);
	NavLeftCommand() {}
	~NavLeftCommand() {}
};

class NavRightCommand : public CommandMenu {
public:
	void execute(RState* state);
	NavRightCommand() {}
	~NavRightCommand() {}
};

class NavUpCommand : public CommandMenu {
public:
	void execute(RState* state);
	NavUpCommand() {}
	~NavUpCommand() {}
};

class NavDownCommand : public CommandMenu {
public:
	void execute(RState* state);
	NavDownCommand() {}
	~NavDownCommand() {}
};

class ConfirmCommand : public CommandMenu {
public:
	void execute(RState* state);
	ConfirmCommand() {}
	~ConfirmCommand() {}
};

class DenyCommand : public CommandMenu {
public:
	void execute(RState* state);
	DenyCommand() {}
	~DenyCommand() {}
};

/*****************************************************************************************************/
class CommandCharSel : public CommandMenu {
public:
	virtual void execute(RState* state) = 0;
	CommandCharSel() {}
	virtual ~CommandCharSel() {}
};

class AddCharacterCommand : public CommandCharSel {
public:
	void execute(RState* state);
	AddCharacterCommand() {}
	~AddCharacterCommand() {}
};

class RemoveCharacterCommand : public CommandCharSel {
public:
	void execute(RState* state);
	RemoveCharacterCommand() {}
	~RemoveCharacterCommand() {}
};

class ChangeCharacterCommand : public CommandCharSel {
public:
	void execute(RState* state);
	ChangeCharacterCommand() {}
	~ChangeCharacterCommand() {}
};

class NextColorCommand : public CommandCharSel {
public:
	void execute(RState* state);
	NextColorCommand() {}
	~NextColorCommand() {}
};

class PrevColorCommand : public CommandCharSel {
public:
	void execute(RState* state);
	PrevColorCommand() {}
	~PrevColorCommand() {}
};

/*****************************************************************************************************/
class CommandPlayer : public Command {
public:
	virtual void execute(RState* state) = 0;
	CommandPlayer() {}
	virtual ~CommandPlayer() {}

	friend class Toggle;
};

class JumpCommand : public CommandPlayer {
public:
	void execute(RState* state);
	JumpCommand() {}
	~JumpCommand() {}

	friend class Toggle;
};

class AttackCommand : public CommandPlayer {
public:
	void execute(RState* state);
	AttackCommand() {}
	~AttackCommand() {}

	friend class Toggle;
};

class MoveLeftCommand : public CommandPlayer {
public:
	void execute(RState* state);
	MoveLeftCommand() {}
	~MoveLeftCommand() {}

	friend class Toggle;
};

class MoveRightCommand : public CommandPlayer {
public:
	void execute(RState* state);
	MoveRightCommand() {}
	~MoveRightCommand() {}

	friend class Toggle;
};