#include "tetromino.h"

void Tetromino::generateNewPiece(Queue* queue) {
	this->pieceID	= queue->getNextPiece();
	this->x			= INITIAL_PIECE_POSITION_X;
	this->y			= INITIAL_PIECE_POSITION_Y;
	this->rotation	= INITIAL_ROTATION_STATE;
	this->isLocked  = false;
}

int Tetromino::getPieceID() {
	return pieceID;
}

void Tetromino::rotate(int dr) {
	rotation	= (4 + dr + rotation) % 4;
}

void Tetromino::setPosition(int &xPos, int &yPos) {
	x = xPos;
	y = yPos;
}

void Tetromino::setRotation(int &r) {
	rotation = r % 4;
}

void Tetromino::moveLeft() {
	x--;
}

void Tetromino::moveRight() {
	x++;
}

void Tetromino::moveDown() {
	y--;
}
