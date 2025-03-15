#pragma once
#ifndef TETROMINO_H
#define TETROMINO_H

#include "pieces.h"
#include "constants.h"
#include "queue.h"

struct Tetromino {
    int x, y;
    int rotation;
    PieceType pieceID;
    bool isLocked = false;
    int pieceSize;

    int  getPieceSize();
    void generateNewPiece(Queue*);  // Generates a new piece based on the queue
    int  getPieceID();
    void rotate(int dr);  // Rotate the piece 90 degrees
    void setPosition(int &x, int &y);  // Set the position of the piece
    void setRotation(int &rotation);  // Set the rotation of the piece
    void moveLeft();
    void moveRight();
    void moveDown();
};

#endif