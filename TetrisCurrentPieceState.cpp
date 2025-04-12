#include "TetrisCurrentPieceState.h"
#include "constants.h"

int Tetromino::getPieceSize() {
	switch (pieceID) {
	case I_PIECE:
		return 4;
	default:
		return 3;
	}
}

void Tetromino::generateNewPiece(TetrisQueue* queue, int pieceRequested = -1) {
	this->pieceID = (pieceRequested == -1) ? (PieceType)queue->getNextPiece() : (PieceType)pieceRequested;
	this->x = INITIAL_PIECE_POSITION_X;
	this->y = INITIAL_PIECE_POSITION_Y;
	this->rotation = INITIAL_ROTATION_STATE;
	this->pieceSize = getPieceSize();
	this->isLocked = false;
}

int Tetromino::getPieceID() {
	return pieceID;
}

void Tetromino::rotate(int dr) {
	rotation = (4 + dr + rotation) % 4;
}

void Tetromino::setPosition(int& xPos, int& yPos) {
	x = xPos;
	y = yPos;
}

void Tetromino::setRotation(int& r) {
	rotation = r % 4;
}

void Tetromino::moveLeft() {
	x--;
}

void Tetromino::moveRight() {
	x++;
}

void Tetromino::moveDown() {
	y++;
}
