/*	Node.h
 *
 *	This is a node for the graph representing the board of a ChainReaction game.
 *	The node contains information about how many balls it contains, and to which
 *	player they belong.
 *
 *	Vasco Portilheiro, 2015
 */

#ifndef _NODE_H_
#define _NODE_H_

#include <vector>

#include "Player.h"

struct Node {
	
	/* Constructor creates empty node */
	Node() {
		numberOfBalls = 0;
		player = nullptr;
	}

	/* List of adjacent nodes */
	std::vector<Node*> nextList;

	/* Number of balls in node */
	int numberOfBalls;

	/* Player currently controling the node. If the node is empty, this
	 * should be nullptr */
	Player* player;

	/* Capacity of node, equal to the number of adjacent nodes.
	 * Note that this should only be called when nextList is properly
	 * initialized. */
	int capacity() {
		return nextList.size();
	}
};

#endif
