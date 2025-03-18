#include <SDL3/SDL.h>
#include <string>
#include <iostream>

#include "board.h"
#include "queue.h"
#include "pieces.h"

//lazy fix
struct PreviousPieceState {
    int id, x, y, rotation;

    PreviousPieceState() {
        id = 0;
        x = 0;
        y = 0;
        rotation = 0;
    }

    void save(int _id, int _x, int _y, int _rotation) {
		id = _id;
        x = _x;
        y = _y;
        rotation = _rotation;
    }
};

PreviousPieceState lastPieceState;
PreviousPieceState ghostPiece;

Board::Board() {
    queue = new Queue();
    currentPiece = new Tetromino();
    queue->generateNewBag();
    queue->previewQueueUpdate();
    currentPiece->generateNewPiece(queue);
	clearBoard();
}

void Board::Clean() {
}

int** Board::getPlayingField() {
	return reinterpret_cast<int**>(pBoard);
}

void Board::clearBoard() {
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			pBoard[i][j] = { 0 };
		}
	}
}

void Board::printBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            std::cout << pBoard[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Board::getRenderColor(SDL_Renderer* renderer, int piece_id) {
    switch (piece_id) {
    case 0:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);      //blank cells are black
        break;
    case 1:
        SDL_SetRenderDrawColor(renderer, 1, 237, 250, 255);  //I piece : CYAN
        break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 72, 93, 197, 255);  //L piece: BLUE
        break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 255, 200, 46, 255); //J piece: ORANGE
        break;
    case 4:
        SDL_SetRenderDrawColor(renderer, 253, 63, 89, 255);  //S piece: RED
        break;
    case 5:
        SDL_SetRenderDrawColor(renderer, 83, 218, 63, 255);  //Z piece: GREEN
        break;
    case 6:
        SDL_SetRenderDrawColor(renderer, 221, 10, 178, 255); //T piece: PURPLE
        break;
    case 7:
        SDL_SetRenderDrawColor(renderer, 254, 251, 52, 255); //:oyes:
        break;
    default:
        std::cerr << "Undefined behavior detected";
        exit(1);
    }
}

void Board::Render(SDL_Renderer* renderer) {
    //render the last 2 lines without the board grid(otherwise the piece would just look weird)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (pBoard[i][j] != 0) {
                SDL_FRect cell = { BOARD_OFFSET_X + j * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

                getRenderColor(renderer, pBoard[i][j]);

                SDL_RenderFillRects(renderer, &cell, 1);

                // Draw grid
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                SDL_RenderRect(renderer, &cell);
            }
        }
    }

    //render the first 20 lines
    for (int i = 2; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            SDL_FRect cell = { BOARD_OFFSET_X + j * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

            getRenderColor(renderer, pBoard[i][j]);

            SDL_RenderFillRects(renderer, &cell, 1);

            // Draw grid
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderRect(renderer, &cell);
        }
    }

    //Render ghost piece
    //DO NOT RENDER GHOST PIECE IF OVERLAPPED BY CURRENTPIECE
    if (ghostPiece.y != currentPiece->y) {
        for (int i = 0; i < currentPiece->pieceSize; i++) {
            for (int j = 0; j < currentPiece->pieceSize; j++) {
                // Skip empty blocks
                if (Piece_Shape[ghostPiece.id][ghostPiece.rotation][i][j] == 0) continue;

                int ghostX = j + ghostPiece.x;
                int ghostY = i + ghostPiece.y;

                // Check if ghost overlaps with the current piece
                bool overlaps = false;
                for (int pi = 0; pi < currentPiece->pieceSize; pi++) {
                    for (int pj = 0; pj < currentPiece->pieceSize; pj++) {
                        if (Piece_Shape[currentPiece->pieceID][currentPiece->rotation][pi][pj] != 0) {
                            int pieceX = pj + currentPiece->x;
                            int pieceY = pi + currentPiece->y;
                            if (pieceX == ghostX && pieceY == ghostY) {
                                overlaps = true;
                            }
                        }
                    }
                }

                if (!overlaps) {
                    int x = BOARD_OFFSET_X + CELL_SIZE * ghostX;
                    int y = BOARD_OFFSET_Y + CELL_SIZE * ghostY;
                    SDL_FRect ghostCell = { x, y, CELL_SIZE, CELL_SIZE };

                    // Render ghost piece only if no overlap
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 100);
                    SDL_RenderFillRect(renderer, &ghostCell);

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderRect(renderer, &ghostCell);
                }
            }
        }
    }

    //other stuff
    queue->Render(renderer);
    stats.Render(renderer);
}


void Board::editCell(int i, int j, int val) {
    pBoard[i][j] = val;
}

bool Board::canMove(int newX, int newY, int newRotation) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Piece_Shape[currentPiece->pieceID][newRotation][i][j] != 0) {
                int boardX = newX + j;
                int boardY = newY + i;  

                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT) {
                    //std::cout << "Out of bounds at (" << boardX << ", " << boardY << ")\n";
                    return false;  // Out of bounds, the move is invalid
                }

                if (boardY >= 0 && pBoard[boardY][boardX] != 0) {
                    //std::cout << "Collision at (" << boardX << ", " << boardY << ")\n";
                    return false;  // The spot is occupied, can't move
                }
            }
        }
    }
    return true; 
}

bool Board::tryRotation(int newRotation, int direction) {
    for (int i = 0; i < 5; ++i) {
        int offsetX = 0, offsetY = 0;

        switch (currentPiece->pieceID) {
        case O_PIECE:
            offsetX = 0; offsetY = 0;
            break;
        case I_PIECE:
            if (direction > 0) {
                offsetX = I_Piece_offsetData[newRotation][i][0];
                offsetY = -I_Piece_offsetData[newRotation][i][1];
            }
            else {
                offsetX = -I_Piece_offsetData[currentPiece->rotation][i][0];
                offsetY = I_Piece_offsetData[currentPiece->rotation][i][1];
            }
            break;
        default:
            if (direction > 0) {
                offsetX = defaultOffsetData[newRotation][i][0];
                offsetY = -defaultOffsetData[newRotation][i][1];
            }
            else {
                offsetX = -defaultOffsetData[currentPiece->rotation][i][0];
                offsetY = defaultOffsetData[currentPiece->rotation][i][1];
            }
            break;
        }

        int newX = currentPiece->x + offsetX;
        int newY = currentPiece->y + offsetY;

        if (canMove(newX, newY, newRotation)) {
            currentPiece->x = newX;
            currentPiece->y = newY;
            currentPiece->rotate(direction);
            return true;
        }
    }

    return false;
}



bool Board::movePiece(MovementType moveType) {
    if (!currentPiece || currentPiece->isLocked || isGameOver) {
        return false;
    }

    lastPieceState.save(currentPiece->pieceID, currentPiece->x, currentPiece->y, currentPiece->rotation);
    deleteOldBlock();

    bool moved = false;
    int newX, newY, newRotate;

    switch (moveType) {
    case MOVE_LEFT:
        newX = currentPiece->x - 1;
        if (canMove(newX, currentPiece->y, currentPiece->rotation)) {
            currentPiece->moveLeft();
            moved = true;
        }
        break;
    case MOVE_RIGHT:
        newX = currentPiece->x + 1;
        if (canMove(newX, currentPiece->y, currentPiece->rotation)) {
            currentPiece->moveRight();
            moved = true;
        }
        break;
    case ROTATE_CW:
        newRotate = (currentPiece->rotation + 1) % 4;
        moved = tryRotation(newRotate, 1);
        break;
    case ROTATE_CCW:
        newRotate = (currentPiece->rotation + 3) % 4;
        moved = tryRotation(newRotate, -1);
        break;
    case ROTATE_180:
        newRotate = (currentPiece->rotation + 2) % 4;
        moved = tryRotation(newRotate, 1);
        break;
    case SOFT_DROP:
        newY = currentPiece->y + 1;
        if (canMove(currentPiece->x, newY, currentPiece->rotation)) {
            currentPiece->moveDown();
            moved = true;
        }
        break;
    case HARD_DROP: {
        int y = currentPiece->y;
        while (canMove(currentPiece->x, y + 1, currentPiece->rotation)) {
            y++;
        }
        currentPiece->setPosition(currentPiece->x, y);
        putBlockInPlace();
        currentPiece->isLocked = true;
        moved = true;
        break;
       }
    case FALSE_HARD_DROP:
        int fy = currentPiece->y;
        while (canMove(currentPiece->x, fy + 1, currentPiece->rotation)) {
            fy++;
        }
        ghostPiece.save(currentPiece->pieceID, currentPiece->x, fy, currentPiece->rotation);
        break;
    }

    
    putBlockInPlace();
    return moved;
}

void Board::deleteOldBlock() {
    int size = currentPiece->getPieceSize();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int id          = lastPieceState.id;
            int boardX      = lastPieceState.x + j;
            int boardY      = lastPieceState.y + i;
            int rotation    = lastPieceState.rotation;
            
            if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT) {
                continue; // Skip invalid positions
            }

            if (Piece_Shape[id][rotation][i][j] != 0) {
                pBoard[boardY][boardX] = 0;
            }
        }
    }
}

void Board::putBlockInPlace() {
    if (!currentPiece) {
        std::cerr << "Error: currentPiece is nullptr!" << std::endl;
        return;
    }

	int size = currentPiece->getPieceSize();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int id = currentPiece->pieceID;
            int boardX = currentPiece->x + j;
            int boardY = currentPiece->y + i;

            if (Piece_Shape[id][currentPiece->rotation][i][j] != 0) {
                pBoard[boardY][boardX] = Piece_Shape[id][currentPiece->rotation][i][j];
            }
        }
    }
}

int Board::checkForLineClear() {
    int lineCleared = 0;

    for (int i = 2; i < BOARD_HEIGHT; i++) {
        bool isFullLine = true;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (pBoard[i][j] == 0) {
                isFullLine = false;
                break;
            }
        } 

        if (isFullLine) {
            //std::cout << "Line clear detected at line#" << i << "\n";
            lineCleared++;

            // shift all rows above down
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    pBoard[k][j] = pBoard[k - 1][j];
                }
            }

            // clear the top row (since everything shifts down)
            for (int j = 0; j < BOARD_WIDTH; j++) {
                pBoard[0][j] = 0;
            }
        }
    }
    if (lineCleared >= 1) {
        stats.Update(lineCleared);
        std::cout << "Line cleared: " << stats.totalLineCleared << "\n";
        std::cout << "Points: " << stats.points << "\n";
        std::cout << "frames/falldown: " << stats.gravity << "\n\n";
    }
    return lineCleared;
}

void Board::GameOver() {
    std::cout << "Game over\n";
    isGameOver = true;
    Clean();
}

void Board::boardUpdate() {
    if (isGameOver) return;

    // Handle locked pieces
    if (currentPiece->isLocked) {
        int clears = checkForLineClear();
        currentPiece->generateNewPiece(queue);
        if (!canMove(currentPiece->x, currentPiece->y, INITIAL_ROTATION_STATE)) {
            GameOver();
            return;
        }
    }
    else {
        if (fTimer >= stats.gravity) {
            fTimer = 0;
            if (!movePiece(SOFT_DROP)) {
                currentPiece->isLocked = true;
            }
        }
        else {
            fTimer++;
        }
    }
    movePiece(FALSE_HARD_DROP);
}


bool Board::isGameGoing() {
    return !isGameOver;
}

//i literally cannot remove this without something going wrong
void Board::moveRight() {
    movePiece(MOVE_RIGHT);
}

void Board::moveLeft() {
    movePiece(MOVE_LEFT);
}

void Board::Rotate() {
    movePiece(ROTATE_CW);
}

void Board::ReversedRotate() {
    movePiece(ROTATE_CCW);
}

void Board::Rotate180() {
    movePiece(ROTATE_180);
}

void Board::HardDrop() {
    movePiece(HARD_DROP);
}

void Board::SoftDrop() {
    movePiece(SOFT_DROP);
}