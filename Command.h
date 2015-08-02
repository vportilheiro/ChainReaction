/*	Command.h
 *
 *	Provides an interface for command-line input from the user.
 *	Valid commands are either quitting commands or moves.
 *
 *	Vasco Portilheiro, 2015
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

struct Command {
	enum CommandType{QUIT, MOVE};
	CommandType type;
	int row, col;
};

struct QuitCommand : Command {
	QuitCommand() {
		type = QUIT;
		row = -1;
		col = -1;
	}
};

struct MoveCommand : Command {
	MoveCommand(int row, int col) {
		type = MOVE;
		this->row = row;
		this->col = col;
	}
};

#endif
