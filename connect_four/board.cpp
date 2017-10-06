#include "board.h"
#include <iostream>
#include <cassert>

Board::Board()
{
	boardSize = SIZE_OF_BOARD + 1;

	memset(board, ' ', sizeof(board));

	columnsFreeSpace.push_back(0); // it's fake element
	for (int i = 1; i < boardSize; ++i)
	{
		columnsFreeSpace.push_back(SIZE_OF_BOARD);
	}
	columnsFreeSpace.push_back(0); // it's fake element
	firstPlayer = secondPlayer = 0;
}

Board::Board(const Board& rhs)
{
	memcpy(board, rhs.board, sizeof(board));

	boardSize = rhs.boardSize;

	for (int i = 0; i <= boardSize; ++i)
	{
		columnsFreeSpace.push_back(rhs.columnsFreeSpace[i]);
	}
}


Board::~Board()
{
}

int Board::size()
{
	return boardSize;
}


size_t Board::makeTurn(char playerSign, int column)
{
//	std::cout << "\n=============makeTurn==============\n";
	board[columnsFreeSpace[column]][column] = playerSign;

	return columnsFreeSpace[column]--;
}

bool Board::freeColumn(int column)
{
	std::cout << "\n Free space " << columnsFreeSpace[column] << std::endl;
	return columnsFreeSpace[column] > 0;
}


void Board::print()
{
	std::cout << "\n Firsst Player score \n" << firstPlayer << "\n Bot score \n" << secondPlayer << std::endl;
	for (int line = 1; line < boardSize; ++line)
	{
		for (int col = 1; col < boardSize; ++col)
		{
			std::cout << " ||" << board[line][col];
		}
		std::cout << " ||\n";
	}
}

bool Board::isFull()
{
	int sum = 0;

	for (int i = 1; i < boardSize; ++i)
	{
		sum += columnsFreeSpace[i];
	}

	return sum == 0;
}


bool Board::checkForWinner(char player, size_t column, size_t line)
{
	assert(column > 0 && column < boardSize && line > 0 && line < boardSize);

	return
		checkHorizontal(player, column, line) ||
		checkVertical(player, column, line) ||
		checkDiagonals(player, column, line);
}

bool Board::checkHorizontal(char player, size_t column, size_t line)
{
	std::cout << "\n=============checkHorizontal==============\n";
	size_t count = 1; // it's for curent element

	// right check
	for (int r = column + 1; r < boardSize; ++r)
	{
		if (board[line][r] == player)
		{
			++count;
		}
		else
		{
			break;
		}
	}

	// left check
	for (int l = column - 1; l > 0; --l)
	{
		if (board[line][l] == player)
		{
			++count;
		}
		else
		{
			break;
		}
	}
	
	return count >= SIZE_OF_WIN_LINE;
}

bool Board::checkVertical(char player, size_t column, size_t line)
{
	size_t count = 1; // it's for curent element
	++line;

	while (line < boardSize && board[line][column] == player)
	{
		std::cout << "\n=============checkVertical==============\n";
		++line;
		++count;
	}

	return count >= SIZE_OF_WIN_LINE;
}

bool Board::checkDiagonals(char player, size_t column, size_t line)
{
	std::cout << "\n=============checkDiagonals==============\n";
	size_t count = 1;

	// Check main diagonal
	for (int l = line - 1, c = column - 1; l > 0 && c > 0; --l, --c)
	{
		if (board[l][c] == player)
		{
			++count;
		}
		else
		{
			break;
		}

	}

	for (int l = line + 1,c = column + 1; l < boardSize && c < boardSize; ++l, ++c)
	{
		if (board[l][c] == player)
		{
			++count;
		}
		else
		{
			break;
		}
	}

	if (count >= SIZE_OF_WIN_LINE)
	{
		return true;
	}
	else
	{
		count = 1;
	}

	// check the other diagonal
	for (int l = line - 1, c = column + 1; l > 0 && c < boardSize; --l, ++c)
	{
		if (board[l][c] == player)
		{
			++count;
		}
		else
		{
			break;
		}
	}

	for (int l = line + 1, c = column - 1; c > 0 && l < boardSize; ++l, --c)
	{
		if (board[l][c] == player)
		{
			++count;
		}
		else
		{
			break;
		}
	}

	return count >= SIZE_OF_WIN_LINE;
}

void Board::clearBoard()
{
	for (int i = 0; i < boardSize; ++i)
	{
		columnsFreeSpace[i] = 0;
	}
}

int Board::getColumnFreeSpace(int column)
{
	assert(column >= 0 && column <= boardSize);

	return columnsFreeSpace[column];
}

void Board::evaluateScore(char player)
{
	if (player == playerOne)
	{
		firstPlayer = 0;
		firstPlayer += getAllHorizontal(player);
		firstPlayer += getAllVertical(player);
	}
	else
	{
		secondPlayer = 0;
		secondPlayer += getAllHorizontal(player);
		secondPlayer += getAllVertical(player);
	}

}

long long Board::getAllHorizontal(char player)
{
	long long score = 0, count = 0;
	size_t coef_4 = 0;
	size_t coef_3 = 0;
	if (player == playerOne)
	{
		coef_4 = FOUR * 10;
		coef_3 = THREE * 10;
	}

	else
	{
		coef_4 = FOUR;
		coef_3 = THREE;
	}

	for (int line = 1; line < boardSize; ++line)
	{
		count = 0;
		for (int column = 1; column < boardSize; ++column)
		{
			if (board[line][column] == player)
			{
				++count;
			}
			else
			{
				if (count == 2)
				{
					score += 5;
				}
				else if (count == 3)
				{
					score += 5 * 5 * 5 * coef_3;
				}
				else if (count == 4)
				{
					score += 5 * 5 * 5 * 5 * coef_4;
				}
				count = 0;
			}
		}
		if (count == 2)
		{
			score += 5;
		}
		else if (count == 3)
		{
			score += 5 * 5 * 5 * coef_3;
		}
		else if (count == 4)
		{
			score += 5 * 5 * 5 * 5 * coef_4;
		}
	}


	return score;
}

long long Board::getAllVertical(char player)
{
	long long score = 0, count = 0;
	size_t coef_4 = 0;
	size_t coef_3 = 0;
	if (player == playerOne)
	{
		coef_4 = FOUR * 10;
		coef_3 = THREE * 10;
	}

	else
	{
		coef_4 = FOUR;
		coef_3 = THREE;
	}


	for (int column = 1; column < boardSize; ++column)
	{
		count = 0;
		for (int line = 1; line < boardSize; ++line)
		{
			if (board[line][column] == player)
			{
				++count;
			}
			else
			{
				if (count == 2)
				{
					score += 5;
				}
				else if (count == 3)
				{
					score += 5 * 5 * 5 * coef_3;
				}
				else if (count == 4)
				{	
					score += 5 * 5 * 5 * 5 * coef_4;
				}
				count = 0;
			}
		}
		if (count == 2)
		{
			score += 5;
		}
		else if (count == 3)
		{
			score += 5 * 5 * 5 * coef_3;
		}
		else if (count == 4)
		{
			score += 5 * 5 * 5 * 5 * coef_4;
		}
	}

	return score;
}

long long Board::getHeuristic()
{
	return secondPlayer - firstPlayer;
}