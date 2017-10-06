#include "alphaBetaTree.h"
#include <iostream>

#include <limits>

AlphaBetaTree::AlphaBetaTree(const Board& initState, size_t depht, Type type) :
	state(initState)
{
	this->depht = depht;
	this->type = type;

	alpha = std::numeric_limits<long long>::min();
	beta = std::numeric_limits<long long>::max();

	bestChoiceIndex = -1;
}


AlphaBetaTree::~AlphaBetaTree()
{
	for (int i = 0; i < newStates.size(); ++i)
	{
		delete newStates[i];
	}
}


// Evaluate tree and return column to choice
int AlphaBetaTree::treeEvaluation()
{
	int columnToMove = -1;

	if (depht == 0 || state.isFull())
	{
		// get eursitic from state
		state.evaluateScore(playerOne);
		state.evaluateScore(playerTwo);
		currenHeuristic = state.getHeuristic();
		return startPosition.second;
	}

	else
	{
		switch (type)
		{
		case MIN:
		{
			setNewStates(playerOne);
			if (bestChoiceIndex == -1)
			{
				searchHeuristicAndBestIndex(MIN);
			}
			return newPositions[bestChoiceIndex].second;
		}
		case MAX:
		{
			setNewStates(playerTwo);
			if (bestChoiceIndex == -1)
			{
				searchHeuristicAndBestIndex(MAX);
			}
			return newPositions[bestChoiceIndex].second;
		}
		}

	}
	return columnToMove;
}

void AlphaBetaTree::searchHeuristicAndBestIndex(Type type)
{
	switch (type)
	{
	case MIN:
		currenHeuristic = std::numeric_limits<long long>::max();
		for (auto i = 0; i < newStates.size(); ++i)
		{
			if (currenHeuristic > newStates[i]->getHeuristic())
			{
				currenHeuristic = newStates[i]->getHeuristic();
				bestChoiceIndex = i;
			}
		}
		break;
	case MAX:
		currenHeuristic = std::numeric_limits<long long>::min();
		for (auto i = 0; i < newStates.size(); ++i)
		{
			if (currenHeuristic < newStates[i]->getHeuristic())
			{
				currenHeuristic = newStates[i]->getHeuristic();
				bestChoiceIndex = i;
			}
		}
		break;
	}
}

long long AlphaBetaTree::getHeuristic()
{
	return currenHeuristic;
}

void AlphaBetaTree::setNewStates(char playerSign)
{
	// generate new position and make new state
	generateNewPositions(playerSign);

	Type newType = (playerSign == playerOne) ? MAX : MIN; // TODO test it

	for (int i = 0; i < newPositions.size(); ++i)
	{
	//	std::cout << "\n Fake Board \n";
		AlphaBetaTree* newTree = new AlphaBetaTree(state, depht - 1, newType);
		
		newTree->state.makeTurn(playerSign, newPositions[i].second);

		newTree->alpha = alpha;
		newTree->beta = beta;

		newTree->startPosition.first = newPositions[i].first;
		newTree->startPosition.second = newPositions[i].second;

		newTree->treeEvaluation();

		// TODO  Да взема евристиката и да сетна алфа или бета и ако се пресичат да спра

		if (type == MAX)
		{
			if (newTree->currenHeuristic > alpha)
			{
				alpha = newTree->currenHeuristic;
				currenHeuristic = newTree->currenHeuristic;
				bestChoiceIndex = i;
			}
		}
		else
		{
			if (newTree->currenHeuristic < beta)
			{
				beta = newTree->currenHeuristic;
				currenHeuristic = newTree->currenHeuristic;
				bestChoiceIndex = i;
			}
		}

		if (alpha > beta)
		{
			delete newTree;
			return;
		}

		newStates.push_back(newTree);
	}
}

void AlphaBetaTree::generateNewPositions(char playerSign)
{
	for (int i = 1; i <= SIZE_OF_BOARD; ++i)
	{
		if (state.getColumnFreeSpace(i) > 0)
		{
			if (i == 1)
			{
				if ((state.getColumnFreeSpace(i + 1) < SIZE_OF_BOARD) ||
						state.getColumnFreeSpace(i) < SIZE_OF_BOARD)
				newPositions.push_back(std::make_pair(state.getColumnFreeSpace(i), i));
			}

			else if (i == SIZE_OF_BOARD )
			{
				if ((state.getColumnFreeSpace(i - 1) < SIZE_OF_BOARD) ||
						state.getColumnFreeSpace(i) < SIZE_OF_BOARD)
				newPositions.push_back(std::make_pair(state.getColumnFreeSpace(i), i));
			}

			else if ((state.getColumnFreeSpace(i - 1) < SIZE_OF_BOARD ||
						state.getColumnFreeSpace(i + 1) < SIZE_OF_BOARD ||
						state.getColumnFreeSpace(i) < SIZE_OF_BOARD))
			{
				newPositions.push_back(std::make_pair(state.getColumnFreeSpace(i), i));
			}
		}
	}

}
