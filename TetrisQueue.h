#pragma once
#ifndef TETRISQUEUE_H
#define TETRISQUEUE_H

#include <vector>
#include <SDL.h>

class TetrisQueue {
public:
	void generateNewBag();
	void previewQueueUpdate();
	int getNextPiece();
	void Reset();
	void Render(SDL_Renderer* renderer, int alpha);
private:
	std::vector<int> bag = { 0,1,2,3,4,5,6 };
	std::vector<int> pieceQueue;
	std::vector<int> previewQueue;
};

#endif
