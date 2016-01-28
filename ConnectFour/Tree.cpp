#include "Tree.h"
#include "Board.h"

Tree::Tree(const Board& rootBoard, int depth) {
	constructTree(rootBoard, depth);
}

Tree::~Tree() {
	releaseMemory(root);
}

Node& Tree::getRoot() {
	return *root;
}

void Tree::constructTree(const Board& rootBoard, int depth) {
	root = new Node();
	root->board = rootBoard;
	constructAllPossibleMoves(*root, depth, BOT);
}

void Tree::constructAllPossibleMoves(
		Node& parentNode, 
		int depth, 
		const uint8_t playerId) {
	if (depth <= 0) {
		return;
	}

	for (int c = 0; c < WIDTH; ++c) {
		if (parentNode.canBePlaced(c)) {
			Node* child = new Node();

			child->board = parentNode.board;
			child->setPlayerMove(c, playerId);
			parentNode.addChild(child);

			constructAllPossibleMoves(
				*child, 
				depth - 1, 
				(playerId == HUMAN ? BOT : HUMAN));
		}
	}
}

void Tree::releaseMemory(Node* n) {
	if (n->isLeaf()) {
		delete n;
		return;
	}

	for (auto child : n->children) {
		releaseMemory(child);
	}

	n->children.clear();
	delete n;
}

