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


class TetrisBoard {
	public:
		CurrentIngameState	state = NORMAL;
		float   GameOverTimestamp;
	
		TetrisBoard();
		~TetrisBoard();
		void    loadAsset(SDL_Renderer* renderer);
		void	Clean();
		void	Render(SDL_Renderer* renderer);
		bool	tryRotation(int newRotation, int direction);
		bool 	movePiece(MovementType movement);
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
		void	clearBoard();
	
		//probably not the best idea to group these together
		void	HoldPiece();
		void	setAlpha(Uint8 v);
	private:
		int			pBoard[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
		int			fTimer;
		int			pieceLockfTimer;
		int			level;
		int 		holdPiece = -1;
		bool		isHoldUsed = false;
		MovementType lastMovement;
		float		alpha = 255.0f;
	
		TetrisQueue* queue = nullptr;
		Tetromino* currentPiece = nullptr;

		void	getRenderColor(SDL_Renderer* renderer, int piece_id, int alpha);
		void    renderHoldPiece(SDL_Renderer* renderer);
		void    renderPlayingBoard(SDL_Renderer* renderer);

		void	deleteOldBlock();
		void	putBlockInPlace();
		int		checkForLineClear(TSpinType type, SDL_Renderer* renderer, Game& game);
		bool	canMove(int x, int y, int r);
		void	editCell(int i, int j, int val);
		void	printBoard();
		TSpinType isTspin();
		bool	isCellOccupied(int x, int y);
};
#endif // !TETRISCHALLENGE_H

