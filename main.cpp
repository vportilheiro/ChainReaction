/*	main.cpp
 *
 *	This is an implementation of the game ChainReaction, found as an game in
 *	the App Store. The game consists of a two-dimensional array -- the "board" --
 *	whose cells may contain a certain number of tokens -- "balls". Each cell has a
 *	capacity equal to the number of cells non-diagonally adjacent to it. (For example,
 *	a corner cell has a capacity of two, any other edge cell a capacity of three, and
 *	other cells a capacity of four.) Players take turns adding a single ball to any cell
 *	that either contains no balls of opposing players (in other words, has no balls,
 *	or only balls placed there by the same player). When a player adds a ball that makes
 *	a cell reach its capacity, that cell "explodes". When a cell explodes, it empties its
 *  balls, adding one to each adjacent cell. If the adjacent cells contain balls of
 *  another player, these become the bals of the player who just went. In this way,
 *  a player may "capture" opponents' balls. The game is one when only one player has
 *  balls on the board.
 *
 *  Note: this file uses copies of getInteger(), getYesOrNo(), and integerToString()
 *  as found in the StanfordCPP library provided by Stanford for the CS 106 B course.
 *
 *	Vasco Portilheiro, 2015
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ChainReaction.h"
#include "colormod.h"
#include "Command.h"
#include "Player.h"

/* If true, will try to print to terminal using ANSI-escaped colors */
static const bool COLOR = true;

/* Prototypes */
void congradulatePlayer(Player const* player);
void deletePlayers(std::vector<Player*>& playerList);
void displayGreeting();
void displayGoodbye();
void getBoardSize(int& rows, int& cols);
Command getCommand(Player* const player);
int getInteger(std::string prompt, std::string reprompt);
void getPlayers(std::vector<Player*>& playerList);
bool getYesOrNo(std::string prompt, std::string reprompt);
std::string integerToString(int n);
bool isQuitCommand(const std::string& command);
bool parseCommand(std::string commandString, Command& command);
bool playAgain();
void printScores(std::vector<Player*>& playerList);

/* This is the command-line interface for the game */
int main() {

	/* Display welcome message */
	displayGreeting();

	/* Get game parameters, including the dimensions of the board,
	 * and the players that will be playing, stored in a vector.
	 * Note that this vector is different than the one the game object
	 * stores. While the game will change its copy to reflect the state
	 * of the game, this copy will persist between games, so as to store
	 * player scores. */
	std::vector<Player*> playerList;
	getPlayers(playerList);
	
	/* If color flag is on, assign each player a color */
	if (COLOR) {

	}

	while (true) {
		int rows, cols;
		getBoardSize(rows, cols);
	
		/* Create game */
		ChainReaction game(rows, cols, playerList, COLOR);
		std::cout << game << std::endl;
	
		/* Loop that runs the game. Will get a command from the player,
		 * which is either an in-bounds location to place a ball,
		 * or a command to quit. */
		//int currentPlayerNum = 0;
		Player* currentPlayer;
		bool gameQuit = false;
		while (!game.gameOver()) {
			//Player* currentPlayer = playerList[currentPlayerNum];
			currentPlayer = game.currentPlayer();
			Command command = getCommand(currentPlayer);
			if (command.type == Command::QUIT) {
				gameQuit = true;
				break;
			} else {
			/* The game then checks if the move is valid. If not, the player is
			 * reprompted. Otherwise, the move is played, and the current player is
			 * updated, based on the players left in the game. */
				int row = command.row;
				int col = command.col;
				if (currentPlayer->move(row, col, game)) {
					std::cout << "Placed ball." << std::endl;
					//currentPlayerNum = (currentPlayerNum + 1) % game.numberOfPlayers();
					std::cout << game << std::endl;
				} else {
					std::cout << "Invalid move. Try again." << std::endl;
				}
			}
		}
		if (gameQuit) {
			std::cout << "The game was quit." << std::endl;
		} else {
			//congradulatePlayer(playerList[currentPlayerNum]);
			//playerList[currentPlayerNum]->win();
			congradulatePlayer(currentPlayer);
			currentPlayer->win();
		}
		printScores(playerList);
		if (!playAgain()) {
			break;
		}
	}
	deletePlayers(playerList);
	displayGoodbye();
	return 0;
}

/* Displays a congradulation to the given player */
void congradulatePlayer(Player const* player) {
	std::cout << "Congradulations " << player->color() << player->getName()
			  << player->uncolor() << "!" << std::endl;
}

/* Frees all memore associated with the players in playerList */
void deletePlayers(std::vector<Player*>& playerList) {
	for (Player* player : playerList) {
		delete player;
	}	
}

/* Displays greeting message to the user */
void displayGreeting() {
	std::cout << std::endl
			  << "	*****************************" << std::endl
			  << "	* Welcome to ChainReaction! *" << std::endl
			  << "	*****************************" << std::endl
			  << "	Vasco Portilheiro, 2015. See main.cpp for rules."
			  << std::endl
			  << std::endl;
}

/* Displays message when program is done executing */
void displayGoodbye(){
	std::cout << "Thanks for playing ChainReaction!" << std::endl;
}

/* Prompt the user for a valid integer number of rows and columns */
void getBoardSize(int& rows, int& cols) {
	std::cout << "Enter the dimensions for the board to play on." << std::endl;
	rows = getInteger("Number of rows: ", "");
	cols = getInteger("Number of columns: ", "");
}

/* Prompts the user for a command, either a move, or quit. Will reprompt
 * until a command is given in the valid format. */ 
Command getCommand(Player* const player) {
	while (true) {
		std::cout << player->color() << player->getName()
				  << " (\"row,column\" or \"quit\"): " << player->uncolor();
		std::string commandString;
		std::getline(std::cin, commandString);
		Command command;
		if (parseCommand(commandString, command)) {
			return command;
		} else {
			std::cout << "Invalid command format. Enter a position to place a ball" 
					  << std::endl << "as a row and column separated by a comma, "
					  << "or \"quit\" to exit." << std::endl;
		}
	}
}

/* Gets a valid integer from the user. This function is a minimally modified
 * version of the same function found in the StanfordCPP library. */
int getInteger(std::string prompt, std::string reprompt) {
	if (reprompt == "") {
		reprompt = "Illegal integer format. Try again.";
	}
	int value;
	std::string line;
	while (true) {
		std::cout << prompt;
		std::getline(std::cin, line);
		std::istringstream stream(line);
		stream >> value;
		if (!stream.fail() && stream.eof()) break;
		std::cout << reprompt << std::endl;
		if (prompt == "") prompt = "Enter an integer: ";
	}
	return value;
}

/* Prompts the user for the number of players, and provides the option to give
 * the players' names. Pointers to these players are stored in the given playerList.
 * These are maxed out at six, if for no other reason than there are only eight
 * printable colors available to distinguish the players. */
void getPlayers(std::vector<Player*>& playerList) {
	int numberOfPlayers = 0;
	while (true) {
	   numberOfPlayers = getInteger("Number of players (max 6): ", "");
	   if (numberOfPlayers >= 1 && numberOfPlayers <= 6)
		   break;
	   std::cout << "Please enter a number between 1 and 6 (inclusive)." << std::endl;
	}
	for (int i = 0; i < numberOfPlayers; ++i) {
		std::string name;
		std::cout << "Player " << i + 1 << "'s name "
				  << "(enter for \"Player " << i + 1 << "\"): ";
		std::getline(std::cin, name);
		//TODO: fix default case -- hangs
		if (name == "")
			name = "Player " + integerToString(i + 1);
		Player* player = new Player(name);
		/* If color flag is on, enable player's color */
		if (COLOR) {
			Color::Code color = Color::colorFromInt(i + 1);
			player->setColor(color);
			player->showColor();
		}
		playerList.push_back(player);
	}
}

/* Prompts the user for a yes or no answer. This function is a minimally modified
 * version of the same function found in the StanfordCPP library. */
bool getYesOrNo(std::string prompt, std::string reprompt) {
	if (reprompt == "") {
		reprompt = "Please type a word that starts with 'Y' or 'N'.";
	}
	bool value;
	std::string line;
	while (true) {
		std::cout << prompt;
		std::getline(std::cin, line);
		if ((int) line.length() > 0) {
			char first = tolower(line[0]);
			if (first == 'y') {
				value = true;
				break;
			} else if (first == 'n') {
				value = false;
				break;
			}
		}
		std::cout << reprompt << std::endl;
		if (prompt == "") prompt = "Try again: ";
		}
	return value;
}

std::string integerToString(int n) {
	std::ostringstream stream;
	stream << n;
	return stream.str();
}

bool isQuitCommand(const std::string& command) {
	return (command == "quit");
}

bool parseCommand(std::string commandString, Command& command) {
	if (isQuitCommand(commandString)) {
		command = QuitCommand();
		return true;
	} else {
		int row, col;
		char comma;
		std::istringstream commandStream(commandString);
		if (commandStream >> row >> comma >> col) {
			command = MoveCommand(row, col);
			return true;
		} else {
			return false;
		}
	}
}

bool playAgain() {
	return getYesOrNo("Would you like to play again? (y/n) ", "");
}

void printScores(std::vector<Player*>& playerList) {
	std::cout << "===== SCORES =====" << std::endl;
	for (Player* player : playerList) {
		std::cout << player->color() << player->getName()
				  << player->uncolor() << ": " << player->getScore() << std::endl;
	}
}
