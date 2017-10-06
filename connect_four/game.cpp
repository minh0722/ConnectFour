#include "game.h"
#include <iostream>
#include <cassert>
#include "alphaBetaTree.h"
#include <SDL.h>

Game::Game()
{
	players[0] = playerOne;
	players[1] = playerTwo;

	onTurn = 0;

	window.init();
	window.blitBoardSurface();
}


Game::~Game()
{
}

void Game::start()
{
	do
	{
		std::cout << "\n Enter one for single player and two for Multiplayer \n";
		std::cin >> playersCount;

	} while (playersCount < 1 && playersCount > 2);

}

bool Game::isFinished()
{
	return board.isFull();
}

void Game::run()
{
	SDL_Event inputEvent;

	while (!isFinished())
	{
		while (SDL_PollEvent(&inputEvent))
		{
			switch (inputEvent.type) {
			case SDL_MOUSEBUTTONDOWN:
				int x = inputEvent.button.x;
				int column = (x / (/*PIECE_WIDTH*/47 + 15)) % 10/*WIDTH*/;
				++column;
				if (column < 1 || column > SIZE_OF_BOARD || board.getColumnFreeSpace(column) == 0)
				{
					break;
				}

				size_t line = board.makeTurn(players[onTurn], column);
				std::cout << line << " " << column << std::endl;

				window.blitPiece(line, column, onTurn);

				if (board.checkForWinner(players[onTurn], column, line))
				{
					board.clearBoard();
					std::cout << "\n We have WINNER Player " << players[onTurn] << std::endl;
					SDL_Delay(3000);
					return;
				}

				onTurn = (onTurn + 1) % 2;

				AlphaBetaTree currentTree(board, DEPHT, MAX);
				column = currentTree.treeEvaluation();

				/*std::cout << "\n Heuristcis : " << currentTree.getHeuristic()<< std::endl;*/
				assert(onTurn >= 0 && onTurn < 2);

				line = board.makeTurn(players[onTurn], column);
				
				window.blitPiece(line, column, onTurn);

				if (board.checkForWinner(players[onTurn], column, line))
				{
					board.clearBoard();
					std::cout << "\n We have WINNER Player " << players[onTurn] << std::endl;
					SDL_Delay(3000);
					return;
				}


				onTurn = (onTurn + 1) % 2;
				std::cout << "\n Heuristcis : " << currentTree.getHeuristic() << std::endl;
				currentTree.state.print();
				print();
				break;
			}

		}
	}
}

void Game::print()
{
	board.print();
}
