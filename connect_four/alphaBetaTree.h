#pragma once
#include "board.h"

class AlphaBetaTree
{
public:
	AlphaBetaTree(const Board& initState, size_t depht, Type type);
	~AlphaBetaTree();

	int treeEvaluation();
	void setNewStates(char player);

	void searchHeuristicAndBestIndex(Type type);

	long long getHeuristic();


	std::pair<int, int> startPosition;
	Board state; // TODO make a private
private:

	void generateNewPositions(char playerSign);

	Type type;
	
	std::vector<AlphaBetaTree*> newStates;
	std::vector<std::pair<int, int> > newPositions;
	size_t depht;
	short bestChoiceIndex;
	long long currenHeuristic;

	size_t alpha;
	size_t beta;
};

