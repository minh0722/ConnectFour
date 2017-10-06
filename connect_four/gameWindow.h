#pragma once

struct SDL_Window;
struct SDL_Surface;

class GameWindow
{
public:
	GameWindow();
	~GameWindow();
	void blitBoardSurface();
	void blitPiece(size_t line, size_t column, size_t player);


	void init();

	SDL_Window* gameWindow;
	SDL_Surface* gameWindowSurface;

	SDL_Surface* boardSurface;
	SDL_Surface* red;
	SDL_Surface* yellow;
};

