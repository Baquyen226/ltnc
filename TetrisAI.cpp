#include <algorithm>
#include <cmath>

#include "game.h"
#include "TetrisAI.h"
#include "TetrisBoard.h"

//void TetrisAI::copyBoardState(int** pBoard) {
//	if (!board) {
//		std::cerr << "Error: TetrisBoard pointer is null in copyBoardState.\n";
//		return;
//	}
//	int** ptr = board->getPlayingBoard();
//	memcpy(boardState, ptr, sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
//}

//(thank you gpt very cool)
//- Try every(x, rotation) placement that can be hard dropped
//	->	clone the board
//	->	place the piece at (x, y) with rotation
//- Evaluate the resulting board using evaluateBoardState()
//- Choose the best scoring move
//- Generate a simple movement plan :
//	->	Rotate to desired orientation
//	->	Move left / right to desired x
//- Hard drop
void TetrisAI::generateMovePlan(SDL_Renderer* renderer, Game& game) {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in generateMovePlan.\n";
		return;
	}
	if (movePlan.size() > 0) {
		return;
	}

	bool isNotPlayer = 0;
	/*std::cerr << "////////////////////////////////////////////////////////\n Generating Move Plan!!!\n////////////////////////////////////////////////////////\n";*/
	float bestScore = -1e9;
	std::deque<MovementType> bestPlan;

	Tetromino* original = board->getPiece();
	int originalX = original->x;
	int originalRot = original->rotation;
	int pieceID = original->pieceID;


	//check all possible x-positions
	for (int x = -3; x < BOARD_WIDTH; x++) {
		for (int rot = 0; rot < 4; rot++) {
			board->deleteOldBlock();
			if (!board->canMove(x, original->y, rot)) continue; // Skip if the piece can't be placed here
			TetrisBoard tempBoard = *board; // Clone the board
			tempBoard.deleteOldBlock();
			tempBoard.getPiece()->setRotation(rot);
			tempBoard.getPiece()->setPosition(x, original->y);
			if (!tempBoard.canMove(x, original->y, rot)) continue;
			tempBoard.HardDrop(); // Place the piece at the new position
			tempBoard.boardUpdate(renderer, game, isNotPlayer);
		
			// Evaluate the board state after placing the piece
			float score = tempBoard.getAI()->evaluateBoardState(renderer, game);
			//std::cerr << "Candidate move at x: " << x << ", rotation: " << rot << ", score: " << score << "\n";
			
			if (score > bestScore) {
				bestPlan = generateMove(originalX, originalRot, x, rot);
				bestScore = score;
				//std::cerr << "Better move found at x: " << x << ", rotation: " << rot << ", score: " << score << "\n";
				//std::cerr << "//////////////////SIMULATED BOARD//////////////////////\n";
				//tempBoard.printBoard();
				//evaluateBoardState(renderer, game, 1); // Print evaluation details
				//std::cerr << "////////////////////////////////////////////////////////\n";
			}
		}
	}
	/*std::cerr << "Best plan found, score: " << bestScore << "\n";*/
	movePlan = bestPlan;
}

std::deque<MovementType> TetrisAI::generateMove(int originalX, int originalRot, int x, int rot) {
	int dx = x - originalX;
	std::deque<MovementType> movePlan;

	// Handle rotation - calculate shortest path
	int rotationsNeeded = (rot - originalRot + 4) % 4;

	switch (rotationsNeeded) {
	case 1:
		movePlan.push_back(ROTATE_CW);
		break;
	case 2:
		movePlan.push_back(ROTATE_180);
		break;
	case 3:
		movePlan.push_back(ROTATE_CCW);
		break;
		// case 0: no rotation needed
	}

	// Handle movement
	if (dx > 0) {
		for (int i = 0; i < dx; i++) {
			movePlan.push_back(MOVE_RIGHT);
		}
	}
	else if (dx < 0) {
		for (int i = 0; i < -dx; i++) {
			movePlan.push_back(MOVE_LEFT);
		}
	}

	movePlan.push_back(HARD_DROP);
	return movePlan;
}

void TetrisAI::executeMovePlan() {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in executeMovePlan.\n";
		return;
	}
	for (const auto& move : movePlan) {
		board->movePiece(move);
	}
}

void TetrisAI::clearMovePlan() {
	movePlan.clear();
}

void TetrisAI::addMovement(MovementType move) {
	movePlan.push_back(move);
}

void TetrisAI::setAlpha(Uint8 alpha) {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in setAlpha.\n";
		return;
	}
	board->setAlpha(alpha);
}

//Evaluate board to the following formula:
//1. minimize hole
//2. only one well	TODO: punish well at column 2 and 8
//3. optimal height would be around 10-12
//4. reward tetris
//5. burn if necessary -> not necessary actually
//kill yourself brother

//i give up bro
float TetrisAI::evaluateBoardState(SDL_Renderer* rendrerer, Game& game, bool printResult) {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in evaluateBoardState.\n";
		return 0.0f;
	}
	getCleanBoardCopy();

	int holes = holeCount();
	int Bumpiness = bumpiness();
	int height = heightAggerate();
	int linesCleared = completeLines();

	return HOLE_WEIGHT * holes
		+ BUMPINESS_WEIGHT * Bumpiness
		+ HEIGHT_WEIGHT * height
		+ COMPLETE_LINES_WEIGHT * linesCleared;
}

void TetrisAI::getCleanBoardCopy() {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in cleanBoardCopy.\n";
		return;
	}
	boardState.clear();
	boardState = board->getPlayingBoard();

	int size = board->getPiece()->getPieceSize();
	int pieceX = board->getPiece()->x;
	int pieceY = board->getPiece()->y;
	int ID = board->getPiece()->pieceID;
	int rot = board->getPiece()->rotation;

	//delete current piece for eval
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (Piece_Shape[ID][rot][i][j] != 0) {
				int x = pieceX + j;
				int y = pieceY + i;
				if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
					boardState[y][x] = 0;
			}
		}
	}
}


int TetrisAI::findHighestCell(int col) {
	int highest = 0;
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in findHighestCell.\n";
		return highest;
	}
	for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
		if (boardState[at(i)][col] != 0) {
			highest = i;
			break;
		}
	}
	return highest;
}

int TetrisAI::heightAggerate() {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in heightAggerate.\n";
		return 0;
	}
	int totalHeight = 0;
	for (int j = 0; j < BOARD_WIDTH; j++) {
		totalHeight += findHighestCell(j);
	}
	return totalHeight;
}

int TetrisAI::bumpiness() {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in flatness.\n";
		return 0.0f;
	}
	float totalFlatness = 0.0f;
	for (int i = 0; i < BOARD_WIDTH - 1; i++) {
		totalFlatness += std::abs(findHighestCell(i) - findHighestCell(i + 1));
	}
	return totalFlatness;
}

int TetrisAI::holeCount() {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in holeCount.\n";
		return 0;
	}
	int holes = 0;
	for (int j = 0; j < BOARD_WIDTH; j++) {
		bool foundBlock = false;
		for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
			if (boardState[at(i)][j] != 0) {
				foundBlock = true;
			}
			else if (foundBlock) {
				holes++;
			}
		}
	}
	return holes;
}

int TetrisAI::completeLines() {
	if (!board) {
		std::cerr << "Error: TetrisBoard pointer is null in completeLines.\n";
		return 0;
	}
	int completeLines = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		bool isComplete = true;
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (boardState[at(i)][j] == 0) {
				isComplete = false;
				break;
			}
		}
		if (isComplete) {
			completeLines++;
		}
	}
	return completeLines;
}