#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include "defines.h"

class Board {
public:
	Board();
	Board(const Board& b);
	Board& operator=(const Board& b);
	
	void setPlayerMove(const uint8_t column, const uint8_t playerId);

	bool canBePlaced(const uint8_t column);

	void printBoard();

	int heuristicValue();

	uint8_t getCellState(uint8_t x, uint8_t y);

	bool gameOver(std::vector<std::pair<int,int>>& winPos, uint8_t playerId);

	bool boardFilled();

private:
	int countVerticalLineWithLengthN(
		int length, 
		uint8_t playerId);

	int countHorizontalLineWithLengthN(
		int length, 
		uint8_t playerId);

	int countDiagonalLineWithLengthN(
		int length, 
		uint8_t playerId);

	/**
	*   NW - northwest
	*   SE - southeast
	*   SW - southwest
	*   NE - northeast
	*/
	int countMainDiagonalNWToSE(
		int length,
		uint8_t playerId);

	int countMainDiagonalSWToNE(
		int length,
		uint8_t playerId);

	int countUpperDiagonalNWToSE(
		int length,
		uint8_t playerId);

	int countUpperDiagonalSWToNE(
		int length,
		uint8_t playerId);

	int countLowerDiagonalNWToSE(
		int length,
		uint8_t playerId);

	int countLowerDiagonalSWToNE(
		int length,
		uint8_t playerId);



	/* Methods for getting winning positions */
	void getVerticalLinesWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int,int>>& winPos);

	void getHorizontalLinesWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	void getDiagonalLinesWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	/**
	*   NW - northwest
	*   SE - southeast
	*   SW - southwest
	*   NE - northeast
	*/
	void getMainDiagonalNWToSEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	void getMainDiagonalSWToNEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	void getUpperDiagonalNWToSEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	void getUpperDiagonalSWToNEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	void getLowerDiagonalNWToSEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);

	void getLowerDiagonalSWToNEWithLength4(
		uint8_t playerId,
		std::vector<std::pair<int, int>>& winPos);


	void printBoardToFile();


	uint8_t table[HEIGHT][WIDTH];
	uint8_t columnHeight[WIDTH];
};
