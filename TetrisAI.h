#pragma once
#ifndef TETRISAI_H
#define TETRISAI_H

#include <iostream>
#include <vector>
#include <deque>
#include <SDL.h>

#include "TetrisPieceData.h"
#include "TetrisQueue.h"
#include "TetrisCurrentPieceState.h"
#include "HelperDataType.h"
#include "constants.h"

class TetrisBoard;
class Game;

// Simple heuristic AI. Reference https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/
class TetrisAI {
public:
	TetrisAI(TetrisBoard* _board) : board(_board) {};

	// AI functions
	// void copyBoardState(int** pBoard);
	void generateMovePlan(SDL_Renderer* renderer, Game& game);
	void executeMovePlan();
	void clearMovePlan();
	void addMovement(MovementType move);
	void setAlpha(Uint8 alpha);
	void getCleanBoardCopy();
	float evaluateBoardState(SDL_Renderer* renderer, Game& game, bool printResult = 0);
	// Getters
	std::deque<MovementType>& getMovePlan() { return movePlan; }
	//const TetrisBoard* getBoard() const { return board; }

private:
	TetrisBoard* board = nullptr;
	std::deque<MovementType> movePlan;
	std::vector<std::vector<int>> boardState;

	// eval helper
	int heightAggerate();
	int bumpiness();
	int holeCount();
	int completeLines();
	int findHighestCell(int col);


	// move generator
	std::deque<MovementType> generateMove(int originalX, int originalRot, int x, int rot);
	//Note: Use this to avoid FREAKING INVERSE INDEXING :sob:
	int at(int _y0) { return BOARD_HEIGHT - _y0 - 1; };
};
#endif