#pragma once

#include <stdint.h>
#include "config.h"
#include <vector>

class Board {
public:
	Board();
	~Board();
	Board(const Board& rhs);

	int size();
	size_t makeTurn(char player, int column);
	bool freeColumn(int column);

	void print();
	bool isFull();
	void clearBoard();

	bool checkForWinner(char player, size_t column, size_t line);

	int getColumnFreeSpace(int column);
	void evaluateScore(char player);

	long long getHeuristic();

	long long getAllHorizontal(char player);
	long long getAllVertical(char player);

private:

	bool checkHorizontal(char player, size_t column, size_t line);
	bool checkVertical(char player, size_t column, size_t line);
	bool checkDiagonals(char player, size_t column, size_t line);

	int boardSize;
	char board[SIZE_OF_BOARD + 1][SIZE_OF_BOARD + 1];

	std::vector<int> columnsFreeSpace;

	long long firstPlayer;
	long long secondPlayer;
};

