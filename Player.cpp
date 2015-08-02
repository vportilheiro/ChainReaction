/*	Player.cpp
 *
 *	Implements the player class, containing information about a player
 *	for ChainReaction. See Player.h for more.
 *
 *	Vasco Portilheiro, 2015
 */

#include "ChainReaction.h"
#include "Player.h"

/* Constructor set the user's name and initializes number of balls to zero,
 * as well as setting the flag that the player hasn't moved yet. */
Player::Player(const std::string& name)
	: colorModifier(Color::DEFAULT, false), name(name), score(0) {}

/*
void Player::changeNumberOfBalls(int change) {
	balls += change;
}
*/

/* Returns colorModifier */
Color::Modifier Player::color() const {
	return colorModifier;
}

/* Sends message to given game to try and place a ball at the given
 * location. Will return whether the move was valid and successful. */
bool Player::move(int row, int col, ChainReaction& game) {
	return game.playerMove(row, col, this);
}

/* Returns player's name */
std::string Player::getName() const {
	return name;
}

/* Returns player's score */
int Player::getScore() const {
	return score;
}

/* Sets player's color */
void Player::setColor(Color::Code color) {
	colorModifier.setCode(color);
}

/* Enable/disable color printing */
void Player::showColor() {
	colorModifier.enable();
}

void Player::hideColor() {
	colorModifier.disable();
}

/* Returns a modifier that restores default coloring. The returned modifier
 * will have the same "enabled" status as the player's colorModifier, ensuring
 * that it is only used in an enviroment where ANSI colors are desirable. */
Color::Modifier Player::uncolor() const {
	return Color::Modifier(Color::DEFAULT, colorModifier.enabled());
}

/* Increments the score */
void Player::win() {
	++score;
}
