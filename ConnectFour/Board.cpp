#include "Board.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>

Board::Board() {
	memset(table, 0, sizeof(table));
	memset(columnHeight, 0, sizeof(columnHeight));
}

Board::Board(const Board& b) {
	memcpy(&table, &b.table, sizeof(b.table));
	memcpy(&columnHeight, &b.columnHeight, sizeof(b.columnHeight));
}

Board& Board::operator=(const Board& b) {
	if (this != &b) {
		memcpy(&table, &b.table, sizeof(b.table));
		memcpy(&columnHeight, &b.columnHeight, sizeof(b.columnHeight));
	}
	return *this;
}

void Board::setPlayerMove(const uint8_t column, const uint8_t playerId) {

	if (!canBePlaced(column)) {
		return;
	}

	++columnHeight[column];
	table[HEIGHT - columnHeight[column]][column] = playerId;

	//	printBoard();
}

bool Board::canBePlaced(const uint8_t column) {
	return columnHeight[column] < HEIGHT;
}

int Board::heuristicValue() {
	int human = countVerticalLineWithLengthN(2, HUMAN) * 50 +
		countVerticalLineWithLengthN(3, HUMAN) * 5000 +
		countHorizontalLineWithLengthN(2, HUMAN) * 50 +
		countHorizontalLineWithLengthN(3, HUMAN) * 5000 +
		countDiagonalLineWithLengthN(2, HUMAN) * 50 +
		countDiagonalLineWithLengthN(3, HUMAN) * 5000;

	int bot = countVerticalLineWithLengthN(2, BOT) * 10 +
		countVerticalLineWithLengthN(3, BOT) * 500 +
		countHorizontalLineWithLengthN(2, BOT) * 10 +
		countHorizontalLineWithLengthN(3, BOT) * 500 +
		countDiagonalLineWithLengthN(2, BOT) * 10 +
		countDiagonalLineWithLengthN(3, BOT) * 500;

	//    printf("human heuristic val: %d\n", human);
	//    printf("bot heuristic val: %d\n", bot);
	//    printf("heuristic val: %d\n", human - bot);
	return bot - human;
}

uint8_t Board::getCellState(uint8_t x, uint8_t y) {
	assert(0 <= x && x < HEIGHT && 0 <= y && y < WIDTH);
	return table[x][y];
}

bool Board::gameOver(std::vector<std::pair<int, int>>& winPos, uint8_t playerId) {
	getVerticalLinesWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return true;
	}

	getHorizontalLinesWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return true;
	}

	getDiagonalLinesWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return true;
	}

	return boardFilled();
}

bool Board::boardFilled() {
	for (size_t x = 0; x < HEIGHT; ++x) {
		for (size_t y = 0; y < WIDTH; ++y) {
			if (table[x][y] == 0) {
				return false;
			}
		}
	}

	return true;
}

int Board::countVerticalLineWithLengthN(
		int length, 
		uint8_t playerId) {

	int count = 0;
	for (size_t c = 0; c < WIDTH; ++c) {
		for (int rowIdx = 0; rowIdx < HEIGHT - length + 1; ++rowIdx) {
			int line = 1;
			for (int r = HEIGHT - rowIdx - 1; r >= HEIGHT - rowIdx - length; --r) {
				line &= (table[r][c] == playerId);
			}

			count += line;
		}
	}
	return count;
}

int Board::countHorizontalLineWithLengthN(
		int length,
		uint8_t playerId) {

	int count = 0;
	for (size_t r = 0; r < HEIGHT; ++r) {
		for (int columnIdx = 0; columnIdx < WIDTH - length + 1; ++columnIdx) {
			int line = 1;
			for (int c = WIDTH - columnIdx - 1; c >= WIDTH - columnIdx - length; --c) {
				line &= (table[r][c] == playerId);
			}

			count += line;
		}
	}
	return count;
}

int Board::countDiagonalLineWithLengthN(
		int length,
		uint8_t playerId) {

	return countMainDiagonalNWToSE(length, playerId) +
		countMainDiagonalSWToNE(length, playerId) +
		countUpperDiagonalNWToSE(length, playerId) +
		countUpperDiagonalSWToNE(length, playerId) +
		countLowerDiagonalNWToSE(length, playerId) +
		countLowerDiagonalSWToNE(length, playerId);
}

int Board::countMainDiagonalNWToSE(
		int length,
		uint8_t playerId) {

    int count = 0;

    size_t diagSize = std::min(WIDTH, HEIGHT);

    for(size_t r = 0; r < diagSize - length + 1; ++r) {
        int line = 1;

		for (size_t x = r; x < r + length; ++x) {
			line &= (table[x][x] == playerId);
		}

		count += line;
    }

	return count;
}

int Board::countMainDiagonalSWToNE(
		int length,
		uint8_t playerId) {

	int count = 0;

	size_t diagSize = std::min(WIDTH, HEIGHT);

	for (int r = diagSize - 1; r >= length - 1; --r) {
		int line = 1;

		for (int x = r; x > r - length; --x) {
			line &= (table[x][diagSize - 1 - x] == playerId);
		}

		count += line;
	}

	return count;
}

int Board::countUpperDiagonalNWToSE(
		int length,
		uint8_t playerId) {

	int count = 0;

	size_t diagSize = std::min(WIDTH, HEIGHT);

	//std::fstream out("log.txt", std::ios::out | std::ios::app);

	//out << "###################################################################" << std::endl;
	for (int c = 1; c <= diagSize - length; ++c) {
		//out << "c = " << c << std::endl;
		for (size_t r = c; r <= diagSize - length ; ++r) {
			//out << "r = " << r << std::endl;
			int line = 1;

			for (size_t x = r; x < r + length; ++x) {
				//out << x << " " << x - c << std::endl;
				line &= (table[x - c][x] == playerId);
			}

			count += line;
		}
	}
	//out << "###################################################################" << std::endl;
	//out.close();
	return count;
}

int Board::countUpperDiagonalSWToNE(
		int length,
		uint8_t playerId) {

	int count = 0;

	size_t diagSize = std::min(WIDTH, HEIGHT);

	for (int c = diagSize - 2; c >= length - 1; --c) {
		for (int r = c; r >= length - 1; --r) {
			int line = 1;

			for (int x = r; x > r - length; --x) {
				line &= (table[x][c - x] == playerId);
			}

			count += line;
		}
	}
	return count;
}

int Board::countLowerDiagonalNWToSE(
		int length,
		uint8_t playerId) {

	int count = 0;

	size_t diagSize = std::min(WIDTH, HEIGHT);

	for (int c = 1; c <= diagSize - length; ++c) {
		for (size_t r = c; r <= diagSize - length; ++r) {
			int line = 1;

			for (size_t x = r; x < r + length; ++x) {
				line &= (table[x][x - c] == playerId);
			}

			count += line;
		}
	}
	return count;
}

int Board::countLowerDiagonalSWToNE(
		int length,
		uint8_t playerId) {

	int count = 0;

	size_t diagSize = std::min(WIDTH, HEIGHT);

	for (int c = 1; c <= diagSize - length; ++c) {
		for (int r = c; r <= diagSize - length; ++r) {
			int line = 1;

			for (int x = r; x < r + length; ++x) {
				line &= (table[x][diagSize + c - 1 - x] == playerId);
			}

			count += line;
		}
	}
	return count;
}

void Board::getVerticalLinesWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	int length = 4;

	for (size_t c = 0; c < WIDTH; ++c) {
		for (int rowIdx = 0; rowIdx < HEIGHT - length + 1; ++rowIdx) {
			int line = 1;
			for (int r = HEIGHT - rowIdx - 1; r >= HEIGHT - rowIdx - length; --r) {
				line &= (table[r][c] == playerId);

				winPos.push_back(std::make_pair(r, c));
			}

			if (!line) {
				winPos.clear();
			}
			else {
				return;
			}

		}
	}
}

void Board::getHorizontalLinesWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	int length = 4;

	for (size_t r = 0; r < HEIGHT; ++r) {
		for (int columnIdx = 0; columnIdx < WIDTH - length + 1; ++columnIdx) {
			int line = 1;
			for (int c = WIDTH - columnIdx - 1; c >= WIDTH - columnIdx - length; --c) {
				line &= (table[r][c] == playerId);

				winPos.push_back(std::make_pair(r, c));
			}

			if (!line) {
				winPos.clear();
			}
			else {
				return;
			}

		}
	}
}

void Board::getDiagonalLinesWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	getMainDiagonalNWToSEWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return;
	}

	getMainDiagonalSWToNEWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return;
	}

	getUpperDiagonalNWToSEWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return;
	}

	getUpperDiagonalSWToNEWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return;
	}

	getLowerDiagonalNWToSEWithLength4(playerId, winPos);
	if (!winPos.empty()) {
		return;
	}

	getLowerDiagonalSWToNEWithLength4(playerId, winPos);
}

void Board::getMainDiagonalNWToSEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	size_t diagSize = std::min(WIDTH, HEIGHT);
	int length = 4;

	for (size_t r = 0; r < diagSize - length + 1; ++r) {
		int line = 1;

		for (size_t x = r; x < r + length; ++x) {
			line &= (table[x][x] == playerId);

			winPos.push_back(std::make_pair(x, x));
		}

		if (!line) {
			winPos.clear();
		}
		else {
			return;
		}

	}
}

void Board::getMainDiagonalSWToNEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	size_t diagSize = std::min(WIDTH, HEIGHT);
	int length = 4;

	for (int r = diagSize - 1; r >= length - 1; --r) {
		int line = 1;

		for (int x = r; x > r - length; --x) {
			line &= (table[x][diagSize - 1 - x] == playerId);

			winPos.push_back(std::make_pair(x, diagSize - 1 - x));
		}

		if (!line) {
			winPos.clear();
		}
		else {
			return;
		}
	}
}

void Board::getUpperDiagonalNWToSEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	size_t diagSize = std::min(WIDTH, HEIGHT);
	int length = 4;

	for (int c = 1; c <= diagSize - length; ++c) {
		for (size_t r = c; r <= diagSize - length; ++r) {
			int line = 1;

			for (size_t x = r; x < r + length; ++x) {
				line &= (table[x - c][x] == playerId);

				winPos.push_back(std::make_pair(x - c, x));
			}

			if (!line) {
				winPos.clear();
			}
			else {
				return;
			}
		}
	}
}

void Board::getUpperDiagonalSWToNEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	size_t diagSize = std::min(WIDTH, HEIGHT);
	int length = 4;

	for (int c = diagSize - 2; c >= length - 1; --c) {
		for (int r = c; r >= length - 1; --r) {
			int line = 1;

			for (int x = r; x > r - length; --x) {
				line &= (table[x][c - x] == playerId);

				winPos.push_back(std::make_pair(x, c - x));
			}

			if (!line) {
				winPos.clear();
			}
			else {
				return;
			}
		}
	}
}

void Board::getLowerDiagonalNWToSEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	size_t diagSize = std::min(WIDTH, HEIGHT);
	int length = 4;

	for (int c = 1; c <= diagSize - length; ++c) {
		for (size_t r = c; r <= diagSize - length; ++r) {
			int line = 1;

			for (size_t x = r; x < r + length; ++x) {
				line &= (table[x][x - c] == playerId);

				winPos.push_back(std::make_pair(x, x - c));
			}

			if (!line) {
				winPos.clear();
			}
			else {
				return;
			}
		}
	}
}

void Board::getLowerDiagonalSWToNEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos) {

	size_t diagSize = std::min(WIDTH, HEIGHT);
	int length = 4;

	for (int c = 1; c <= diagSize - length; ++c) {
		for (int r = c; r <= diagSize - length; ++r) {
			int line = 1;

			for (int x = r; x < r + length; ++x) {
				line &= (table[x][diagSize + c - 1 - x] == playerId);

				winPos.push_back(std::make_pair(x, diagSize + c - 1 - x));
			}

			if (!line) {
				winPos.clear();
			}
			else {
				return;
			}
		}
	}
}


void Board::printBoard() {
	for (int r = 0; r < HEIGHT; ++r) {
		for (int c = 0; c < WIDTH; ++c) {
			std::cout << (int)table[r][c] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void Board::printBoardToFile() {
	std::ofstream out("game_log.txt", std::ios::out | std::ios::app);

	for (int r = 0; r < HEIGHT; ++r) {
		for (int c = 0; c < WIDTH; ++c) {
			out << (int)table[r][c] << " ";
		}
		out << std::endl;
	}
	out << std::endl;

	out.close();
}