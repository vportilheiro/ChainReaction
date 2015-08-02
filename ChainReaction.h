/*	ChainReaction.h
 *
 *	This file contains the class representing the game ChainReaction, as found in the App
 *	Store. The game consists of a two-dimensional array -- the "board" --
 *	whose cells may contain a certain number of tokens -- "balls". Each cell has a
 *	capacity equal to the number of cells non-diagonally adjacent to it. (For example,
 *	a corner cell has a capacity of two, any other edge cell a capacity of three, and
 *	other cells a capacity of four.) Players take turns adding a single ball to any cel
 *	that either contains no balls of opposing players (in other words, has no balls,
 *	or only balls placed there by the same player). When a player adds a ball that make
 *	a cell reach its capacity, that cell "explodes". When a cell explodes, it empties i
 *	balls, adding one to each adjacent cell. If the adjacent cells contain balls of
 *	another player, these become the bals of the player who just went. In this way,
 *	a player may "capture" opponents' balls. A player loses when all their balls have
 *	been captured in this way. The last player alive is the winner.
 *
 *	To make the chain reaction calculations easier, the board is stored as a graph of
 *	nodes containing information on the balls at each location. However, since these
 *	nodes need to be accessible by row and column coordinateds, pointers to these
 *	nodes are stored in a two-dimensional array representing the board. (This array
 *	is also used to help initialize the graph with its connections.)
 *
 *	Vasco Portilheiro, 2015
 */

#ifndef _CHAINRXN_H_
#define _CHAINRXN_H_

#include <iostream>
#include <map>
#include <vector>

#include "colormod.h"
#include "Node.h"
#include "Player.h"

static const bool BOLD_CAPACITY = true;

class ChainReaction {
public:

	/* Constructor initializes the board, and thus takes the number
	 * of rows and columns. It also takes a list of players, which it will
	 * copy locally. */
	ChainReaction(int rows, int cols, const std::vector<Player*>& playerList,
				  bool colorsEnabled = false);

	/* Deallocates memory associated with the board */
	~ChainReaction();

	/* Returns whether the game is over */
	bool gameOver() const;

	/* Returns the number of players left alive in the game */
	int numberOfPlayers() const;

	/* Attempts to place a ball at the given position for the given player.
	 * Will return true if successful, and false otherwise. The latter is the
	 * case if the position is out of bounds, or the player may not place the
	 * ball at the position, do to there already being another player's balls there. */
	bool playerMove(int row, int col, Player* player);

	friend std::ostream& operator <<(std::ostream& out,
									 const ChainReaction& game);

private:
	
	struct PlayerData;
	using PlayerDataMapT = std::map<Player*, PlayerData>;

	/* Number of rows and columns in the grid */
	const int rows;
	const int cols;

	/* Allows user to specify whether color printing is enabled. Note that player
	 * color enabling overrides this. */
	const bool colorsEnabled;

	/* Array representing the board, which contains pointers to the corresponding
	 * nodes at each location. */
	Node** nodeGrid;

	/* Structure to keep track of player specific data, such the number of balls
	 * any player has on the board, and whether it's a player's first move */
	struct PlayerData {
		PlayerData(): numberOfBalls(0), firstMove(true) {}
		int numberOfBalls;
		int firstMove;
	};

	/* Dictionary from players to their data. For a regular, human vs human game
	 * this data consists of the number of balls each player has on the board.
	 * When this number reaches zero the player is removed from the dictionary.
	 * Similarly, the game is over when only one player is left.
	 * For games agains the computer, this will also contain a set of valid
	 * moves for each player, in order to make searching for optimal plays
	 * more efficient, not having to re-calculate the valid moves each time. */
	PlayerDataMapT playerData;

	/* Adds a ball of the given player to the node, and calculates any resulting
	 * chain reactions */
	void addBallToNode(Node* node, Player* player);

	/* Updates the player's ball counts when the given player captures the 
	 * given node */
	void captureNode(Node* node, Player* player);

	/* "Explodes" a node when it has reached its capacity. */
	void explode(Node* node);

	/* Function that turns a row and a columns in to the corresponding index of the
	 * nodeGrid array */
	int index(int row, int col) const;

	/* Initialize the player data map using the given list of players.
	 * This is (and should only by) called by the constructor. */
	PlayerDataMapT initPlayerData(const std::vector<Player*>& playerList);

	/* Return whether a position in in bounds (on the board) */
	bool isInBounds(int row, int col) const;

	/* Checks whether a player may place a ball at the given position */
	bool isValidMove(int row, int col, Player const* player) const;

	/* Updates the game's list of players. The list contains "active" players,
	 * in other words, those who have not yet lost. If a player has no balls left
	 * on the board, they are removed from the list. (Note that an exception is
	 * made for the first move, and this each player has a flag for whether they
	 * have played their first move yet.) */
	void updatePlayers();

};

#endif
