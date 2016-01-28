#include <iostream>
#include "Tree.h"
#include "Board.h"
#include "Game.h"
#include "defines.h"
#include <SDL.h>
using namespace std;


SDL_Window* gameWindow = NULL;
SDL_Surface* gameWindowSurface = NULL;

bool initSDL() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Can't init SDL! SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		gameWindow = SDL_CreateWindow(
						"Connect 4", 
						SDL_WINDOWPOS_UNDEFINED, 
						SDL_WINDOWPOS_UNDEFINED, 
						SCREEN_WIDTH, 
						SCREEN_HEIGHT, 
						SDL_WINDOW_SHOWN);

		if (gameWindow == NULL) {
			std::cout << "Can't init SDL! SDL_Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			gameWindowSurface = SDL_GetWindowSurface(gameWindow);
		}
	}

	return success;
}

void deinitSDL() {
	//Deallocate surface
	//SDL_FreeSurface(gHelloWorld);
	//gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int alphaBetaPrune(Node& n, bool isMax, int& myMoveCol/*, int alpha, int beta*/) {
	if (n.isLeaf()) {
		myMoveCol = n.newMoveCol;
		return n.heuristicValue();
	}

	if (isMax) {
		int max_val = INT_MIN;

		for (const auto child : n.children) {
			int col;

			int val = alphaBetaPrune(*child, false, col/*, alpha, beta*/);

			//			alpha = max(val, alpha);

			if (max_val <= val) {
				max_val = val;
				myMoveCol = child->newMoveCol;
			}

			//			if(beta <= alpha) {
			//                break;
			//			}
		}
		return max_val;
	}
	else {
		int min_val = INT_MAX;

		for (const auto& child : n.children) {
			int col;

			int val = alphaBetaPrune(*child, true, col/*, alpha, beta*/);

			//			beta = min(beta, val);

			if (min_val >= val) {
				min_val = val;
				myMoveCol = child->newMoveCol;
			}

			//			if(beta <= alpha) {
			//                break;
			//			}
		}
		return min_val;
	}
}


int main(int argc, char* args[]) {

	//int column;

	//Board b;

	//while (true) {
	//	cin >> column;
	//	--column;

	//	b.setPlayerMove(column, HUMAN);

	//	Tree t(b, 4);

	//	//
	//	//        clock_t begin, end;
	//	//        double time_spent;
	//	//
	//	//        begin = clock();
	//	//
	//	////        int alpha = INT_MIN,
	//	////            beta = INT_MAX;
	//	//
	//	int botCol;
	//	alphaBetaPrune(t.getRoot(), true, botCol/*, alpha, beta*/);
	//	//
	//	//        end = clock();
	//	//        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	//	//
	//	//        FILE* log = fopen("log.txt", "a");
	//	//        char msg[100];
	//	//        snprintf(msg, 100, "Took %.10f\n", time_spent);
	//	//        fputs(msg, log);
	//	//        fclose(log);
	//	//

	//	b.setPlayerMove(botCol, BOT);

	//	b.printBoard();

	//	std::cout << b.countVerticalLineWithLengthN(2, HUMAN) << std::endl;
	//	std::cout << b.countHorizontalLineWithLengthN(3, HUMAN) << std::endl;
	//	std::cout << b.countVerticalLineWithLengthN(2, BOT) << std::endl;
	//	std::cout << b.countHorizontalLineWithLengthN(3, BOT) << std::endl;

	//}



	if (!initSDL()) {
		std::cout << "Failed to init SDL" << std::endl;
	}
	else {
		//run game here		
		Game game(gameWindow);
		game.run();
	}

	deinitSDL();

return 0;
}