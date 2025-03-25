#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <fstream>

#include "board.h"
#include "pieces.h"
#include "sfx.h"

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
    currentPiece->generateNewPiece(queue, -1);
    clearBoard();

    std::ifstream file("config.txt");
    if (!file.is_open()) return;

    file.close();
}


void Board::loadAsset(SDL_Renderer* renderer) {
    state_box.loadFont("asset/font/8-bit.ttf", 40);
    MENU_BUTTON = Button("Menu", "asset/sprite/button.png", renderer);
    RETRY_BUTTON = Button("Menu", "asset/sprite/button.png", renderer);
    std::string BG1 = "asset/picture/BG-01.jpg";
    std::string BG2 = "asset/picture/BG-02.jpg";
    std::string BG3 = "asset/picture/BG-03.jpg";
    std::string BG4 = "asset/picture/BG-04.jpg";
    BG = {Background(BG1, renderer),
          Background(BG2, renderer),
          Background(BG3, renderer),
          Background(BG4, renderer)};
}

void Board::Clean() {
    if (queue != NULL && currentPiece != NULL) {
        delete queue;
        delete currentPiece;
        queue = NULL;
        currentPiece = NULL;
    }
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
    //render bg first
    if (state == NORMAL) {
        for (auto& bg : BG) bg.Update(SDL_GetTicks());
    }
    if (stats.level >= 20) BG[3].Render(renderer);
    else if (stats.level >= 14) BG[2].Render(renderer);
    else if (stats.level >= 6) BG[1].Render(renderer);
    else BG[0].Render(renderer);


    //render a low opacity black filler(bg gets too bright)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_FRect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);


    //render hold piece
    PieceType holdPieceID = (PieceType)holdPiece;
    SDL_FRect holdPieceContainer = { HOLD_PIECE_OFFSET_X, HOLD_PIECE_OFFSET_Y, CELL_SIZE * 5, CELL_SIZE * 4 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &holdPieceContainer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

                getRenderColor(renderer, holdPieceID + 1);
                SDL_RenderFillRect(renderer, &cell);

                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
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

    //render in-game button if state is not normal
    if (state != NORMAL) {
        //Render box
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        std::string s = (state == PAUSED) ? "Paused" : "Game Over";

        SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
        state_box.loadTextToTexture(s, COLOR_WHITE, renderer);

        float x = TEXT_GAMEOVER_POSX_CENTER - state_box.get_width() / 2;
        float y = TEXT_GAMEOVER_POSX_CENTER - state_box.get_height() / 2;
        float w = state_box.get_width() + 50;
        float h = state_box.get_height() + 50;

        SDL_FRect placeholder = { x - 20, y - 20, w, h };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &placeholder);

        state_box.Render(x, y, renderer, 0);

        //Render buttons
        y = y - 90;
        SDL_FRect dest = { x, y, BUTTON_SPRITE_SIZE, BUTTON_SPRITE_SIZE };
        SDL_FRect src = { BUTTON_SPRITE_FACTOR * MENU_BUTTON_X, BUTTON_SPRITE_FACTOR * MENU_BUTTON_Y, 16, 16 };
        MENU_BUTTON.Render(renderer, &src, &dest);

        x = x + state_box.get_width() - 30;
        dest = { x, y, BUTTON_SPRITE_SIZE, BUTTON_SPRITE_SIZE };
        src = { BUTTON_SPRITE_FACTOR * RETRY_BUTTON_X, BUTTON_SPRITE_FACTOR * RETRY_BUTTON_Y, 16, 16 };
        RETRY_BUTTON.Render(renderer, &src, &dest);

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
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



bool Board::movePiece(MovementType moveType) {
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

void Board::deleteOldBlock() {
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
    state = GAMEOVER;
    std::string death = "asset/sound/death.wav";
    sfx.playSound(death, GAME_EVENT);
}

void Board::boardUpdate() {
    if (state != NORMAL) return;

    // Handle locked pieces
    if (currentPiece->isLocked) {
        isHoldUsed = false;
        int clears = checkForLineClear();
        currentPiece->generateNewPiece(queue, -1);
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

void Board::Reset() {
    clearBoard();
    holdPiece = -1;
    queue->Reset();
    stats.Reset();
    currentPiece->generateNewPiece(queue, -1);

    std::cerr << "Game restarted!\n";
    state = NORMAL;
}

std::string move = "asset/sound/move.wav";
//i literally cannot remove this without something going wrong
void Board::moveRight() {
    if(movePiece(MOVE_RIGHT)) sfx.playSound(move, PIECE_MOVING);
}

void Board::moveLeft() {
    if(movePiece(MOVE_LEFT)) sfx.playSound(move, PIECE_MOVING);
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

std::string hold = "asset/sound/hold.wav";

void Board::HoldPiece() {
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
    sfx.playSound(hold, PIECE_ROTATE);
    isHoldUsed = true;
}