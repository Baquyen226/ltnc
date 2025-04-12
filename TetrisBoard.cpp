#include "TetrisBoard.h"
#include "game.h"

#include <SDL3/SDL.h>
#include <string>
#include <iostream>
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

TetrisBoard::TetrisBoard() {
    queue = new TetrisQueue();
    currentPiece = new Tetromino();
    queue->generateNewBag();
    queue->previewQueueUpdate();
    currentPiece->generateNewPiece(queue, -1);
    clearBoard();
}

TetrisBoard::~TetrisBoard() {
    if (queue) delete queue;
    queue = NULL;
    if (currentPiece) delete currentPiece;
    currentPiece = NULL;
}


void TetrisBoard::loadAsset(SDL_Renderer* renderer) {
    return;
}

void TetrisBoard::Clean() {
    if (queue != NULL && currentPiece != NULL) {
        delete queue;
        delete currentPiece;
        queue = NULL;
        currentPiece = NULL;
    }
}


void TetrisBoard::clearBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            pBoard[i][j] = { 0 };
        }
    }
}

void TetrisBoard::printBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            std::cout << pBoard[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void TetrisBoard::getRenderColor(SDL_Renderer* renderer, int piece_id, int alpha) {
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

void TetrisBoard::Render(SDL_Renderer* renderer) {
    //render hold piece
    PieceType holdPieceID = (PieceType)holdPiece;
    SDL_FRect holdPieceContainer = { HOLD_PIECE_OFFSET_X, HOLD_PIECE_OFFSET_Y, CELL_SIZE * 5, CELL_SIZE * 4 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
    SDL_RenderRect(renderer, &holdPieceContainer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_RenderFillRect(renderer, &holdPieceContainer);

    int size = (holdPieceID == I_PIECE) ? 4 : 3;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (Piece_Shape[holdPieceID][0][i][j] != 0) {
                //Pieces that has block at the first row on spawn state: S, Z, T
                bool firstRowOccupied = (holdPieceID == S_PIECE || holdPieceID == Z_PIECE || holdPieceID == T_PIECE);
                //Pieces (except I piece) that has the first column occupied
                bool firstColumnOccupied = !(holdPieceID == I_PIECE || holdPieceID == O_PIECE);

                int cellSize_posX = HOLD_PIECE_OFFSET_X + firstColumnOccupied * CELL_SIZE + ((holdPieceID == I_PIECE) ? 0.5 : 0) * CELL_SIZE + CELL_SIZE * j + ((holdPieceID == O_PIECE) ? 0.5 : 0) * CELL_SIZE,
                    cellSize_posY = HOLD_PIECE_OFFSET_Y + firstRowOccupied * CELL_SIZE + ((holdPieceID == I_PIECE) ? 0.5 : 0) * CELL_SIZE + CELL_SIZE * i;

                SDL_FRect cell = { cellSize_posX, cellSize_posY, CELL_SIZE, CELL_SIZE };

                getRenderColor(renderer, holdPieceID + 1, alpha);
                SDL_RenderFillRect(renderer, &cell);

                SDL_SetRenderDrawColor(renderer, 50, 50, 50, alpha);
                SDL_RenderRect(renderer, &cell);
            }
        }
    }
    SDL_FRect holdPieceCell = { HOLD_PIECE_OFFSET_X + CELL_SIZE, HOLD_PIECE_OFFSET_Y + CELL_SIZE, CELL_SIZE, CELL_SIZE };

    //render the last 2 lines without the board grid(otherwise the piece would just look weird)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (pBoard[i][j] != 0) {
                SDL_FRect cell = { BOARD_OFFSET_X + j * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

                getRenderColor(renderer, pBoard[i][j], alpha);

                SDL_RenderFillRects(renderer, &cell, 1);

                // Draw grid
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, alpha);
                SDL_RenderRect(renderer, &cell);
            }
        }
    }

    //render the first 20 lines
    for (int i = 2; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            SDL_FRect cell = { BOARD_OFFSET_X + j * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

            getRenderColor(renderer, pBoard[i][j], alpha);

            SDL_RenderFillRects(renderer, &cell, 1);

            // Draw grid
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, alpha);
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
                    int a = alpha * 128 / 255;
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, a);
                    SDL_RenderFillRect(renderer, &ghostCell);

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, a);
                    SDL_RenderRect(renderer, &ghostCell);
                }
            }
        }
    }

    //other stuff
    queue->Render(renderer, alpha); 
}


void TetrisBoard::editCell(int i, int j, int val) {
    pBoard[i][j] = val;
}

bool TetrisBoard::canMove(int newX, int newY, int newRotation) {
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

bool TetrisBoard::tryRotation(int newRotation, int direction) {
    for (int i = 0; i < 5; ++i) {
        int offsetX = 0, offsetY = 0;
        //0(currentRotation)->1(newRotation) uses index 0 when referencing the kick table when rotating clockwise              
        //1(currentRotation)->0(newRotation) also uses index 0 when referencing the kick table when rotating counter-clockwise
        //therefore CW uses currenRotation, CCW uses newRotation, except its multiplied by -1(direction)

        switch (currentPiece->pieceID) {
        case O_PIECE:
            offsetX = 0; offsetY = 0;
            break;
        case I_PIECE:
            if (direction > 0) {
                offsetX = I_Piece_offsetData[currentPiece->rotation][i][0];
                offsetY = -I_Piece_offsetData[currentPiece->rotation][i][1];
            }
            else {
                offsetX = -I_Piece_offsetData[newRotation][i][0];
                offsetY = I_Piece_offsetData[newRotation][i][1];
            }
            break;
        default:
            if (direction > 0) {
                offsetX = defaultOffsetData[currentPiece->rotation][i][0];
                offsetY = -defaultOffsetData[currentPiece->rotation][i][1];
            }
            else {
                offsetX = -defaultOffsetData[newRotation][i][0];
                offsetY = defaultOffsetData[newRotation][i][1];
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



bool TetrisBoard::movePiece(MovementType moveType) {
    if (!currentPiece || currentPiece->isLocked) {
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

void TetrisBoard::deleteOldBlock() {
    int size = currentPiece->getPieceSize();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int id = lastPieceState.id;
            int boardX = lastPieceState.x + j;
            int boardY = lastPieceState.y + i;
            int rotation = lastPieceState.rotation;

            if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT) {
                continue; // Skip invalid positions
            }

            if (Piece_Shape[id][rotation][i][j] != 0) {
                pBoard[boardY][boardX] = 0;
            }
        }
    }
}

void TetrisBoard::putBlockInPlace() {
    if (!currentPiece) {
        std::cerr << "Error: currentPiece is nullptr!" << "\n";
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

int TetrisBoard::checkForLineClear(SDL_Renderer* renderer, Game &game) {
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
        game.getStats()->Update(lineCleared, renderer);
        std::cout << "Line cleared: " << game.getStats()->totalLineCleared << "\n";
        std::cout << "Points: " << game.getStats()->points << "\n";
        std::cout << "frames/falldown: " << game.getStats()->gravity << "\n\n";
    }
    return lineCleared;
}

void TetrisBoard::GameOver() {
    std::cout << "Game over\n";
    state = GAMEOVER;
    GameOverTimestamp = SDL_GetTicks();
}

void TetrisBoard::boardUpdate(SDL_Renderer* renderer, Game& game) {
    if (state != NORMAL) return;

    // Handle locked pieces
    if (currentPiece->isLocked) {
        isHoldUsed = false;
        int clears = checkForLineClear(renderer, game);
        currentPiece->generateNewPiece(queue, -1);
        if (!canMove(currentPiece->x, currentPiece->y, INITIAL_ROTATION_STATE)) {
            GameOver();
            return;
        }
    }
    else {
        if (fTimer >= game.getStats()->gravity) {
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

void TetrisBoard::Reset(Game& game) {
    clearBoard();
    holdPiece = -1;
    queue->Reset();
    game.getStats()->Reset();
    currentPiece->generateNewPiece(queue, -1);

    std::cerr << "Game restarted!\n";
    state = NORMAL;
}


//i literally cannot remove this without something going wrong
void TetrisBoard::moveRight() {
    movePiece(MOVE_RIGHT);
}

void TetrisBoard::moveLeft() {
    movePiece(MOVE_LEFT);
}

void TetrisBoard::Rotate() {
    movePiece(ROTATE_CW);
}

void TetrisBoard::ReversedRotate() {
    movePiece(ROTATE_CCW);
}

void TetrisBoard::Rotate180() {
    movePiece(ROTATE_180);
}

void TetrisBoard::HardDrop() {
    movePiece(HARD_DROP);
}

void TetrisBoard::SoftDrop() {
    movePiece(SOFT_DROP);
}

void TetrisBoard::HoldPiece() {
    if (isHoldUsed) return;

    lastPieceState.save(currentPiece->pieceID, currentPiece->x, currentPiece->y, currentPiece->rotation);
    deleteOldBlock();
    if (holdPiece == -1) {
        holdPiece = currentPiece->pieceID;
        currentPiece->generateNewPiece(queue, -1);
    }
    else {
        int temp = holdPiece;
        holdPiece = currentPiece->pieceID;
        currentPiece->generateNewPiece(queue, temp);
    }
    isHoldUsed = true;
}

void TetrisBoard::setAlpha(Uint8 v) {
    alpha = v;
}