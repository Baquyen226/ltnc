#ifndef TETRISCURRENTPIECESTATE_H
#define TETRISCURRENTPIECESTATE_H

#include "TetrisPieceData.h"
#include "TetrisQueue.h"

struct Tetromino {
	int x = 0, y = 0;
	int rotation = 0;
	PieceType pieceID = O_PIECE;
	bool isLocked = false;
	int pieceSize;

	int  getPieceSize();
	void generateNewPiece(TetrisQueue*, int);  // Generates a new piece based on the queue
	int  getPieceID();
	void rotate(int dr); 
	void setPosition(int& x, int& y);  // Set the position of the piece
	void setRotation(int& rotation);  // Set the rotation of the piece
	void moveLeft();
	void moveRight();
	void moveDown();
};

#endif