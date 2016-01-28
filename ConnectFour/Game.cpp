#include "Game.h"
#include "Board.h"
#include "Tree.h"
#include <SDL.h>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <iostream>

SDL_Rect Game::pos[HEIGHT][WIDTH] = {
	{ { 7, 7, 47, 47 }, { 67, 7, 47, 47 }, { 127, 7, 47, 47 }, { 187, 7, 47, 47 }, { 247, 7, 47, 47 }, { 307, 7, 47, 47 }, { 367, 7, 47, 47 }, { 427, 7, 47, 47 }, { 487, 7, 47, 47 }, { 547, 7, 47, 47 } },
	{ { 7, 67, 47, 47 }, { 67, 67, 47, 47 }, { 127, 67, 47, 47 }, { 187, 67, 47, 47 }, { 247, 67, 47, 47 }, { 307, 67, 47, 47 }, { 367, 67, 47, 47 }, { 427, 67, 47, 47 }, { 487, 67, 47, 47 }, { 547, 67, 47, 47 } },
	{ { 7, 127, 47, 47 }, { 67, 127, 47, 47 }, { 127, 127, 47, 47 }, { 187, 127, 47, 47 }, { 247, 127, 47, 47 }, { 307, 127, 47, 47 }, { 367, 127, 47, 47 }, { 427, 127, 47, 47 }, { 487, 127, 47, 47 }, { 547, 127, 47, 47 } },
	{ { 7, 187, 47, 47 }, { 67, 187, 47, 47 }, { 127, 187, 47, 47 }, { 187, 187, 47, 47 }, { 247, 187, 47, 47 }, { 307, 187, 47, 47 }, { 367, 187, 47, 47 }, { 427, 187, 47, 47 }, { 487, 187, 47, 47 }, { 547, 187, 47, 47 } },
	{ { 7, 247, 47, 47 }, { 67, 247, 47, 47 }, { 127, 247, 47, 47 }, { 187, 247, 47, 47 }, { 247, 247, 47, 47 }, { 307, 247, 47, 47 }, { 367, 247, 47, 47 }, { 427, 247, 47, 47 }, { 487, 247, 47, 47 }, { 547, 247, 47, 47 } },
	{ { 7, 307, 47, 47 }, { 67, 307, 47, 47 }, { 127, 307, 47, 47 }, { 187, 307, 47, 47 }, { 247, 307, 47, 47 }, { 307, 307, 47, 47 }, { 367, 307, 47, 47 }, { 427, 307, 47, 47 }, { 487, 307, 47, 47 }, { 547, 307, 47, 47 } },
	{ { 7, 367, 47, 47 }, { 67, 367, 47, 47 }, { 127, 367, 47, 47 }, { 187, 367, 47, 47 }, { 247, 367, 47, 47 }, { 307, 367, 47, 47 }, { 367, 367, 47, 47 }, { 427, 367, 47, 47 }, { 487, 367, 47, 47 }, { 547, 367, 47, 47 } },
	{ { 7, 427, 47, 47 }, { 67, 427, 47, 47 }, { 127, 427, 47, 47 }, { 187, 427, 47, 47 }, { 247, 427, 47, 47 }, { 307, 427, 47, 47 }, { 367, 427, 47, 47 }, { 427, 427, 47, 47 }, { 487, 427, 47, 47 }, { 547, 427, 47, 47 } },
	{ { 7, 487, 47, 47 }, { 67, 487, 47, 47 }, { 127, 487, 47, 47 }, { 187, 487, 47, 47 }, { 247, 487, 47, 47 }, { 307, 487, 47, 47 }, { 367, 487, 47, 47 }, { 427, 487, 47, 47 }, { 487, 487, 47, 47 }, { 547, 487, 47, 47 } },
	{ { 7, 547, 47, 47 }, { 67, 547, 47, 47 }, { 127, 547, 47, 47 }, { 187, 547, 47, 47 }, { 247, 547, 47, 47 }, { 307, 547, 47, 47 }, { 367, 547, 47, 47 }, { 427, 547, 47, 47 }, { 487, 547, 47, 47 }, { 547, 547, 47, 47 } }
};

Game::Game(SDL_Window* window) : gameWindow(window) {
	assert(initSurfaces() == true);
}

Game::~Game() {
	SDL_FreeSurface(boardSurface);
	SDL_FreeSurface(redPlayerSurface);
	SDL_FreeSurface(yellowPlayerSurface);
	SDL_FreeSurface(greenTickSurface);
}


void Game::run() {
	bool quit = false;

	SDL_Event inputEvent;
	std::vector<std::pair<int, int>> winPos;

	while (!quit) {

		while (SDL_PollEvent(&inputEvent) && !quit) {
			switch (inputEvent.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				int x = inputEvent.button.x;
				int col = (x / (PIECE_WIDTH + 15)) % WIDTH;

				if (b.canBePlaced(col)) {
					b.setPlayerMove(col, HUMAN);

					if (b.gameOver(winPos, HUMAN)) {
						quit = true;
						break;
					}

					Tree t(b, 4);

					int botCol;
					int alpha = INT_MIN,
						beta = INT_MAX;
					alphaBetaPrune(t.getRoot(), true, botCol, alpha, beta);

					b.setPlayerMove(botCol, BOT);

					if (b.gameOver(winPos, BOT)) {
						quit = true;
					}
				}

				//b.printBoard();
				break;
			}
		}

		drawBoard();
		drawGameOver(winPos);
	}
	SDL_Delay(3000);
}

void Game::drawBoard() {
	SDL_BlitSurface(boardSurface, nullptr, gameWindowSurface, nullptr);

	for (size_t x = 0; x < HEIGHT; ++x) {
		for (size_t y = 0; y < WIDTH; ++y) {
			uint8_t cellState = b.getCellState(x, y);
			
			if (cellState == HUMAN) {
				SDL_BlitSurface(
					redPlayerSurface, 
					nullptr, 
					gameWindowSurface,
					&pos[x][y]);
			}
			else if (cellState == BOT) {
				SDL_BlitSurface(
					yellowPlayerSurface,
					nullptr,
					gameWindowSurface,
					&pos[x][y]);
			}
		}
	}

	SDL_UpdateWindowSurface(gameWindow);
}

void Game::drawGameOver(std::vector<std::pair<int, int>>& winPos) {
	for (auto& it : winPos) {
		SDL_BlitSurface(
			greenTickSurface,
			nullptr,
			gameWindowSurface,
			&pos[it.first][it.second]);
	}
	SDL_UpdateWindowSurface(gameWindow);
}

bool Game::initSurfaces() {
	bool success = true;

	gameWindowSurface = SDL_GetWindowSurface(gameWindow);
	boardSurface = SDL_LoadBMP("Resources/board.bmp");
	redPlayerSurface = SDL_LoadBMP("Resources/red_circle.bmp");
	yellowPlayerSurface = SDL_LoadBMP("Resources/yellow_circle.bmp");
	greenTickSurface = SDL_LoadBMP("Resources/tick.bmp");

	if (!boardSurface || !redPlayerSurface || !yellowPlayerSurface || !greenTickSurface) {
		printf("Failed to load image! SDL error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

int Game::alphaBetaPrune(Node& n, bool isMax, int& myMoveCol, int alpha, int beta) {
	if (n.isLeaf()) {
		return n.heuristicValue();
	}

	if (isMax) {
		int max_val = INT_MIN;

		for (const auto& child : n.children) {
			int col;

			int val = alphaBetaPrune(*child, false, col, alpha, beta);

			if (max_val < val) {
				myMoveCol = child->newMoveCol;
			}

			max_val = std::max(max_val, val);
			alpha = std::max(alpha, max_val);


			if(beta <= alpha) {
			    break;
			}
		}
		return max_val;
	}
	else {
		int min_val = INT_MAX;

		for (const auto& child : n.children) {
			int col;

			int val = alphaBetaPrune(*child, true, col, alpha, beta);
			
			if (min_val > val) {
				myMoveCol = child->newMoveCol;
			}

			min_val = std::min(min_val, val);
			beta = std::min(beta, min_val);


			if(beta <= alpha) {
			    break;
			}
		}
		return min_val;
	}
}