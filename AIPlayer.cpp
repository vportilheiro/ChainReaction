/*	AIPlayer.cpp
 *
 *	Implements a computer player for ChainReaction. See AIPlayer.h for more information.
 *
 *	Vasco Portilheiro, 2015
 */

#include <limits>

#include "AIPlayer.h"

/* Helper function for minimax search */
Position AIPlayer::alphaBeta(ChainReaction& game) {
	int alpha = std::numeric_limits<int>::max();
	Position bestMove;
	alphaBeta(this, game, 0, 0, bestMove);
	return bestMove;
}

void AIPlayer::alphaBeta(Player* current, ChainReaction game,
						 int alpha, int beta, Position& bestMove) {
}
