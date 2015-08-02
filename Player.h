/*	Player.h
 *
 *	This file provides a basic class for a player in the
 *	ChainReaction game. The information contained by a player is
 *	their name, score (for when multiple games are played), and
 *	the number of balls they have on the board. The last entry is
 *	used by the game to find out when a player has lost. The player
 *	also has a flag for whether they have made their first move yet.
 *	This is to allow the game to remove players when they have no 
 *	balls left on the board, with the exception of the first move.
 *
 *	Vasco Portilheiro, 2015
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>

#include "colormod.h"

class ChainReaction;

class Player {
public:

	/* Constructor takes the player's name */
	Player(const std::string& name);

	/* Changes the number of balls belonging to the player */
	//void changeNumberOfBalls(int change);

	/* Returns a colorModifier that if enabled, will make all streams
	 * through it be the player's color. */
	Color::Modifier color() const;

	/* Sends the given game the requested move. Returns true if the move
	 * is valid. */
	bool move(int row, int col, ChainReaction& game);

	/* Returns the player's name */
	std::string getName() const;

	/* Returns player's score */
	int getScore() const;

	/* Sets the color of the player in the case that colors are enabled. */
	void setColor(Color::Code color);

	/* Enables/disables color modifier, note that this will not remove
	 * any modifiers already used. Rather, it is a way to make any prints
	 * through color() print regular text */
	void showColor();
	void hideColor();

	/* Returns a modifier that will unset the color as set by color() */
	Color::Modifier uncolor() const;

	/* Increments the score by one */
	void win();

private:

	/* If colors are enabled, the color associated with the player */
	Color::Modifier colorModifier;

	/* Player's name */
	std::string name;

	/* Number of games player has won */
	int score;
};

#endif
