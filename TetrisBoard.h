#pragma once
#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include "HelperDataType.h"
#include "TetrisPieceData.h"
#include "TetrisCurrentPieceState.h"
#include "TetrisQueue.h"
#include "IngameStats.h"
#include "texture.h"
#include "constants.h"

class Game;

enum MovementType {
	MOVE_LEFT,
	MOVE_RIGHT,
	ROTATE_CW,
	ROTATE_CCW,
	ROTATE_180,
	SOFT_DROP,
	HARD_DROP,
	FALSE_HARD_DROP //for ghost piece
};

class TetrisBoard {
	public:
		CurrentIngameState	state = NORMAL;
		float   GameOverTimestamp;
	
		TetrisBoard();
		~TetrisBoard();
		void    loadAsset(SDL_Renderer* renderer);
		void	Clean();
		void	clearBoard();
		void	printBoard();
		void	getRenderColor(SDL_Renderer* renderer, int piece_id, int alpha);
		void	Render(SDL_Renderer* renderer);
		void	editCell(int i, int j, int val);
		bool	canMove(int x, int y, int r);
		bool	tryRotation(int newRotation, int direction);
		bool 	movePiece(MovementType movement);
		void	deleteOldBlock();
		void	putBlockInPlace();
		int		checkForLineClear(SDL_Renderer* renderer, Game& game);
		void	boardUpdate(SDL_Renderer* renderer, Game& game);
		void	GameOver();
		void	Reset(Game& game);
	
		//:(
		void	moveRight();
		void	moveLeft();
		void	Rotate();
		void	ReversedRotate();
		void	Rotate180();
		void	HardDrop();
		void	SoftDrop();
	
		//probably not the best idea to group these together
		void	HoldPiece();
		void	setAlpha(Uint8 v);
	private:
		int			pBoard[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
		int			fTimer;
		int			level;
		int 		holdPiece = -1;
		bool		isHoldUsed = false;
		float		alpha = 255.0f;
	
		TetrisQueue* queue = NULL;
		Tetromino* currentPiece = NULL;
};
#endif // !TETRISCHALLENGE_H

