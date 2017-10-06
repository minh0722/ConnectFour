#include "gameWindow.h"
#include "config.h"
#include "windowCfg.h"
#include <iostream>
#include <cassert>

#include <SDL.h>


GameWindow::GameWindow() : gameWindow(nullptr), gameWindowSurface(nullptr)
{
	red = SDL_LoadBMP("resources/red_circle.bmp");
	assert(red != nullptr);
	yellow = SDL_LoadBMP("resources/yellow_circle.bmp");
	assert(yellow != nullptr);
	boardSurface = SDL_LoadBMP("resources/board.bmp");
	assert(boardSurface != nullptr);
}


GameWindow::~GameWindow()
{
	SDL_FreeSurface(red);
	SDL_FreeSurface(yellow);
	SDL_FreeSurface(boardSurface);

	SDL_Quit();
}


void GameWindow::init()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "\n SDL ERROR : " << SDL_GetError() << std::endl;
		exit(0);
	}
	gameWindow = SDL_CreateWindow(
		"Connect 4",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (gameWindow == NULL)
	{
		std::cout << "\n SDL ERROR : " << SDL_GetError() << std::endl;
		exit(0);
	}

	gameWindowSurface = SDL_GetWindowSurface(gameWindow);
}


void GameWindow::blitBoardSurface()
{
	SDL_BlitSurface(boardSurface, nullptr, gameWindowSurface, nullptr);
	SDL_UpdateWindowSurface(gameWindow);
}

void GameWindow::blitPiece(size_t line, size_t column, size_t player)
{
	if (player == 0)
	{
		SDL_BlitSurface(red, nullptr, gameWindowSurface, &positions[line][column]);
	}

	else
	{
		SDL_BlitSurface(yellow, nullptr, gameWindowSurface, &positions[line][column]);
	}
	std::cout << " LInE :" << line << "Column : " << column << std::endl;
	SDL_UpdateWindowSurface(gameWindow);
}