#pragma once

#include "defines.h"
#include "Board.h"
#include "SDL_rect.h"

struct SDL_Window;
struct SDL_Surface;
struct Node;

class Game {
public:
	Game(SDL_Window* window);
	Game& operator=(const Game&) = delete;
	Game(const Game&) = delete;
	~Game();

	void run();

	static SDL_Rect pos[HEIGHT][WIDTH];

private:
	int alphaBetaPrune(Node& n, bool isMax, int& myMoveCol, int alpha, int beta);

	bool initSurfaces();
	void drawBoard();
	void drawGameOver(std::vector<std::pair<int,int>>& winPos);

	SDL_Window* gameWindow;
	SDL_Surface* gameWindowSurface;

	SDL_Surface* boardSurface;
	SDL_Surface* redPlayerSurface;
	SDL_Surface* yellowPlayerSurface;
	SDL_Surface* greenTickSurface;

	Board b;
};