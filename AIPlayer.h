/*	AIPlayer.h
 *
 *	This extends the Player class for a game of ChainReaction to a non-human player.
 *	It uses minimax with alpha-beta pruning to search for the optimal move.
 *
 *	Vasco Portilheiro, 2015
 */

#ifndef _AIPLAYER_H_
#define _AIPLAYER_H_

#include "ChainReaction.h"
#include "Player.h"

/* Struct to hold position, consisting of a row and column */
struct Position {
	Position(int row, int col) : row(row), col(col) {}
	Position() : Position(-1,-1) {}
	int row;
	int col;
};

class AIPlayer : Player {
public:

	/* Constructor */
	//AIPlayer(){}

	/* Desctructor */
	~AIPlayer(){}

	/* Alpha-beta pruned search for best move. Will return the move as a Position.
	 * Takes the game for which the move is to be found. */
	Position alphaBeta(ChainReaction& game);

private:

	/* Recursive alpha-beta pruned minimax search, takes the information from the public
	 * helper funciton, the player's whose turn is being examined and a Position which,
	 * it modifies with the best available move */
	void alphaBeta(Player* current, ChainReaction game, int alpha, int beta, Position& bestMove);

};

#endif
