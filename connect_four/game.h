#pragma once

#include "board.h"
#include <stdint.h>
#include "gameWindow.h"

class Game {
public:
	Game();
	~Game();

	void start();
	bool isFinished();
	void run();

	void print();

private:
	Board board;
	GameWindow window;
	int playersCount;

	char players[2];

	int onTurn;
};

