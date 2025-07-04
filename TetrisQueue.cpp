#include <ctime>
#include <random>
#include <iostream>
#include <cstdlib>
#include <SDL.h>

#include "TetrisPieceData.h"
#include "TetrisQueue.h"
#include "constants.h"

void getRenderColor(SDL_Renderer* renderer, int piece_id, int alpha) {
    switch (piece_id) {
    case 0:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);      //blank cells are black
        break;
    case 1:
        SDL_SetRenderDrawColor(renderer, 1, 237, 250, alpha);  //I piece : CYAN
        break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 72, 93, 197, alpha);  //L piece: BLUE
        break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 255, 200, 46, alpha); //J piece: ORANGE
        break;
    case 4:
        SDL_SetRenderDrawColor(renderer, 253, 63, 89, alpha);  //S piece: RED
        break;
    case 5:
        SDL_SetRenderDrawColor(renderer, 83, 218, 63, alpha);  //Z piece: GREEN
        break;
    case 6:
        SDL_SetRenderDrawColor(renderer, 221, 10, 178, alpha); //T piece: PURPLE
        break;
    case 7:
        SDL_SetRenderDrawColor(renderer, 254, 251, 52, alpha); //:oyes:
        break;
    default:
        std::cerr << "Undefined behavior detected";
        exit(1);
    }
}

void TetrisQueue::generateNewBag() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(bag.begin(), bag.end(), g);
    for (int i : bag) {
        pieceQueue.push_back(i);
    }
}

void TetrisQueue::previewQueueUpdate() {
    previewQueue.clear();

    while (pieceQueue.size() < PIECE_PREVIEW) {
        generateNewBag();
    }

    for (int i = 0; i < PIECE_PREVIEW; i++) {
        if (pieceQueue.empty()) generateNewBag();
        previewQueue.push_back(pieceQueue.at(i));
    }

    pieceQueue.erase(pieceQueue.begin());
}

int TetrisQueue::getNextPiece() {
    if (pieceQueue.empty() && previewQueue.empty()) {
        std::cerr << "Cannot find next piece";
        exit(-1);
    }

    int nextPiece = previewQueue.at(0);
    previewQueueUpdate();
    return nextPiece;
}

void TetrisQueue::Reset() {
    previewQueue.clear();
    pieceQueue.clear();
    generateNewBag();
    previewQueueUpdate();
}

void TetrisQueue::Render(SDL_Renderer* renderer, int alpha) {
    const int OFFSET_Y = 100;

    SDL_FRect previewRect = { PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y, CELL_SIZE * 4, CELL_SIZE * (3 * PIECE_PREVIEW + 1)};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_RenderFillRects(renderer, &previewRect, 1);

    for (int i = 0; i < PIECE_PREVIEW; i++) {
        int pieceID = previewQueue.at(i);

        //Pieces that has block at the first row on spawn state: S, Z, T
        bool firstRowOccupied = (pieceID == S_PIECE || pieceID == Z_PIECE || pieceID == T_PIECE) ? true : false;
        //Pieces (except I piece) that has the first column occupied
        bool firstColumnOccupied = !(pieceID == I_PIECE || pieceID == O_PIECE) ? true : false;

        int size = (pieceID == I_PIECE) ? 4 : 3,
            x = PREVIEW_OFFSET_X + firstColumnOccupied * CELL_SIZE / 2,
            y = PREVIEW_OFFSET_Y + CELL_SIZE * 3 * i + firstRowOccupied * CELL_SIZE + ((pieceID == I_PIECE) ? 0.5 : 0) * CELL_SIZE;
        //surely this is not too confusing( the I piece just cant align with the two first row/cl checks:( )

        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < size; k++) {
                if (Piece_Shape[pieceID][0][k][j] != 0) {
                    SDL_FRect cell = {
                        x + CELL_SIZE * j,
                        y + CELL_SIZE * k,
                        CELL_SIZE, CELL_SIZE
                    };

                    getRenderColor(renderer, pieceID + 1, alpha);
                    SDL_RenderFillRects(renderer, &cell, 1);

                    // Draw grid
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, alpha);
                    SDL_RenderRect(renderer, &cell);
                }
            }
        }
    }
}
