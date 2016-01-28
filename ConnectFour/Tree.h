#pragma once
#include <cstdint>
#include <vector>
#include "Board.h"

struct Node {
	Board board;
	std::vector<Node*> children;
	uint8_t newMoveCol;

	void setPlayerMove(const uint8_t column, const uint8_t playerId) {
		newMoveCol = column;
		board.setPlayerMove(column, playerId);
	}

	bool canBePlaced(const uint8_t column) {
		return board.canBePlaced(column);
	}

	void addChild(Node* child) {
		children.push_back(child);
	}

	bool isLeaf() const {
		return children.empty();
	}

	int heuristicValue() {
		return board.heuristicValue();
	}

};

class Tree {
public:
	Tree(const Board& rootBoard, int depth);
	~Tree();

	Node& getRoot();

private:
	void constructTree(const Board& rootBoard, int depth);

	void constructAllPossibleMoves(
		Node& parentNode,
		int depth,
		const uint8_t playerId);

	void releaseMemory(Node* n);

	Node* root;
};
