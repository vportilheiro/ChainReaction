/*	AIPlayer.cpp
 *
 *	Implements a computer player for ChainReaction. See AIPlayer.h for more information.
 *
 *	Vasco Portilheiro, 2015
 */

#include <limits>

#include "AIPlayer.h"

const int INFINITY = 1000000;

/* Helper function for minimax search */
Position AIPlayer::alphaBeta(ChainReaction& game) {
	int alpha = std::numeric_limits<int>::max();
	Position bestMove;
	ChainReaction originGame = game;
	alphaBeta(game, (-1) * INFINITY, INFINITY, DEPTH, bestMove);
	return bestMove;
}

int AIPlayer::alphaBeta(ChainReaction& game,
						 int alpha, int beta, int depth, Position& bestMove) {
	if (depth == 0 || game.gameOver()){
		return gameValue(game);
	}

	int value;
	/* Maximizing alpha */
	if (game.currentPlayer() == this) {
		value = (-1) * INFINITY;
		/* TODO: change move search from naive index brute-forcing */
		for (int row = 0; row < game.rows; ++row) {
			for (int col = 0; col < game.cols; ++col) {
				ChainReaction childGame = game;
				if (game.currentPlayer()->move(row, col, childGame)) {
					int childAlphaBeta = alphaBeta(childGame, alpha, beta, depth - 1, bestMove);
					if (childAlphaBeta > value)
						value = childAlphaBeta;
						bestMove.set(row, col);
					if (value > alpha)
						alpha = value;
					if (beta <= alpha)
						break;
				}
			}
		}
		return value;
	} else {
		value = INFINITY;
		for (int row = 0; row < game.rows; ++row) {
			for (int col = 0; col < game.cols; ++col) {
				ChainReaction childGame = game;
				if (game.currentPlayer()->move(row, col, childGame)) {
					int childAlphaBeta = alphaBeta(childGame, alpha, beta, depth - 1, bestMove);
					if (childAlphaBeta < value) {
						value = childAlphaBeta;
						bestMove.set(row, col);
					if (value < beta)
						beta = value;
					if (alpha >= beta)
						break;
					}
				}
			}
		}
		return value;
	}
}

int AIPlayer::gameValue(const ChainReaction& game) {
	if (game.gameOver()) {
		if (this == game.winner) {
			return INFINITY;
		} else {
			return (-1) * INFINITY;
		}
	} else {
		return 0;
		// TODO: implement value based on number of balls
		// return game.playerData[ 
	}
}	
