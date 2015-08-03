/*	ChainReaction.cpp
 *
 *	Implements the game logic for ChainReaction. See ChainReaction.h for more.
 *
 *	Vasco Portilheiro, 2015
 */

#include "ChainReaction.h"
#include "Node.h"

/* Constructor will initialize the board, which is represented by a graph
 * of Nodes containing the revelant information about the balls placed.
 * This graph is accessible through a two-dimensional array of pointers
 * to those nodes. The constructor will also create a local list of the
 * players, which it will update to reflect the players still in the game. */
ChainReaction::ChainReaction(int rows, int cols, 
							 const std::vector<Player*>& playerList, bool colors) :
							 rows(rows), cols(cols), colorsEnabled(colors),
							 currentPlayerIdx(0),
							 playerData(initPlayerData(playerList)) {

	/* Create grid of empty Nodes */
	nodeGrid = new Node*[rows * cols];
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			nodeGrid[index(row, col)] = new Node();
		}
	}
	/* Initialize adjacency lists for each Node */
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			Node* currentNode = nodeGrid[index(row, col)];
			for (int shift = -1; shift <= 1; shift += 2) {
				if (isInBounds(row + shift, col)) {
					currentNode->nextList.push_back(nodeGrid[index(row + shift, col)]);
				}
				if (isInBounds(row, col + shift)) {
					currentNode->nextList.push_back(nodeGrid[index(row, col + shift)]);
				}
			}
		}
	}
}

/* Destructor will deallocation memory associated with the board */
ChainReaction::~ChainReaction() {
	/* Delete nodes */
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			delete nodeGrid[index(row, col)];
		}
	}
	delete nodeGrid;
}

/* Gets pointer to current player by using the index of the player
 * in the player data map */
Player* ChainReaction::currentPlayer() {
	auto playerIt = playerData.begin();
	std::advance(playerIt, currentPlayerIdx);
	return playerIt->first;
}

/* Returns whether the game is over, by checking how many players are left */
bool ChainReaction::gameOver() const {
	return (playerData.size() == 1);
}

/* Returns number of player currently in game */
int ChainReaction::numberOfPlayers() const {
	return playerData.size();
}

/* Will attempt to place a ball at the given position. Returns false if the
 * move is invalid. Otherwise, will place the ball and calculate and chain
 * reactions coming from the move, and finally return true. */
bool ChainReaction::playerMove(int row, int col, Player* player) {
	if (isValidMove(row, col, player)) {
		PlayerData& data = playerData[player];
		if (data.firstMove) {
			data.firstMove = false;
		}
		++(data.numberOfBalls);
		Node* node = nodeGrid[index(row, col)];
		addBallToNode(node, player);
		updatePlayers();
		return true;
	}
	return false;
}

/* ===== Private Functions =====*/

/* Adds a ball of the given player to the node, and calculates any
 * ensuing chain reactions */
void ChainReaction::addBallToNode(Node* node, Player* player) {
	if (node->player == nullptr)
		node->player = player;
	if (node->numberOfBalls + 1 == node->capacity()) {
		explode(node);
	} else {
		++(node->numberOfBalls);
	}
}

/* Will changes players' ball counts to reflect the given player
 * capturing the given node. */
void ChainReaction::captureNode(Node* node, Player* capturingPlayer) {
	int changedBalls = node->numberOfBalls;
	--(playerData[node->player].numberOfBalls);
	++(playerData[capturingPlayer].numberOfBalls);
	node->player = capturingPlayer;
}

/* "Explodes" a given node when it has reached its capacity. The node is
 * emptied, and a ball of the given player added to each adjacent node.
 * If the adjacent nodes belong to other players, they are changed to the
 * new player, and the player's ball counts respectively updated. */
void ChainReaction::explode(Node* node) {
	Player* capturingPlayer = node->player;
	node->numberOfBalls = 0;
	node->player = nullptr;
	for (Node* nextNode : node->nextList) {
		if (nextNode->player != nullptr && nextNode->player != capturingPlayer) {
			captureNode(nextNode, capturingPlayer);
		}
		addBallToNode(nextNode, capturingPlayer);
	}
}

/* Returns the one-dimesional index in nodeGrid for given coordinates */
int ChainReaction::index(int row, int col) const {
	return (row * cols) + col;
}

/* Initializes the map from players to their data given a list of players */
ChainReaction::PlayerDataMapT
ChainReaction::initPlayerData(const std::vector<Player*>& playerList) {
	PlayerDataMapT playerData;
	for (Player* player : playerList) {
		playerData.emplace(player, PlayerData());
	}
	return playerData;
}	

/* Returns whether given coordinates are in bounds */
bool ChainReaction::isInBounds(int row, int col) const {
	return ((0 <= row && row < rows) && (0 <= col && col < cols));
}

/* Checks whether a player may place a ball at the given position.
 * Returns false if the coordinates are out of bounds, or if the position
 * already contains another players' balls. */
bool ChainReaction::isValidMove(int row, int col, Player const* player) const {
	if (isInBounds(row, col)) {
		Node* node = nodeGrid[index(row, col)];
		return (node->player == nullptr || (node->player == player));
	}
	return false;
}

/* Removes any players with no balls on the board after the first move
 * from the playerData */
void ChainReaction::updatePlayers() {
	for (std::map<Player*, PlayerData>::iterator it = playerData.begin();
		 it != playerData.end();) {
		Player* player = it->first;
		PlayerData& data = it->second;
		if (!data.firstMove && data.numberOfBalls == 0) {
			playerData.erase(it++);
		} else {
			++it;
		}
	}
	currentPlayerIdx = (currentPlayerIdx + 1) % numberOfPlayers();
}


/* ===== Operators ===== */

std::ostream& operator <<(std::ostream& out, const ChainReaction& game) {
	Color::Modifier bold(Color::BOLD, game.colorsEnabled);
	Color::Modifier unbold(Color::DEFAULT, game.colorsEnabled);
	/* Top edge */
	for (int i = 0; i < game.cols; ++i) {
		out << " ___";
	}
	out << std::endl;

	/* Rows */
	for (int i = 0; i < game.rows; ++i) {
		out << "|";
		for (int j = 0; j < game.cols; ++j) {
			Node* node = game.nodeGrid[game.index(i,j)];
			if (node->player != nullptr) {
				int balls = node->numberOfBalls;
				out << " ";
				if (balls == node->capacity()) {
					out << bold;
				}
				out << node->player->color() << balls << node->player->uncolor();
				if (balls == node->capacity()) {
					out << unbold;
				}
				out << " |";
			} else {
				out << "   |";
			}
		}
		out << std::endl;
		out << "|";
		for (int j = 0; j < game.cols; ++j) {
			out << "___|";
		}
		out << std::endl;
	}
	return out;
}
