#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <vector>

class Queue {
public:
	void generateNewBag();
	void previewQueueUpdate();
	int getNextPiece();
	void Render(SDL_Renderer* renderer);
private:
	std::vector<int> bag = { 0,1,2,3,4,5,6 };
	std::vector<int> pieceQueue;
	std::vector<int> previewQueue;
};

#endif