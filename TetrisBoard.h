#pragma once
#ifndef TETRISBOARD_H
#define TETRISBOARD_H

//yeah lets pretend that this class isnt like 999gb

#include "HelperDataType.h"
#include "TetrisPieceData.h"
#include "TetrisCurrentPieceState.h"
#include "TetrisQueue.h"
#include "IngameStats.h"
#include "texture.h"
#include "constants.h"
#include "TetrisAI.h"

class Game;

class TetrisBoard {
	public:
		CurrentIngameState	state = NORMAL;
		float   GameOverTimestamp;
	
		TetrisBoard();
		TetrisBoard(const TetrisBoard& other);
		~TetrisBoard();

		void    loadAsset(SDL_Renderer* renderer);
		void	Clean();
		void	Render(SDL_Renderer* renderer);
		void	deleteOldBlock();
		bool	tryRotation(int newRotation, int direction);
		bool	canMove(int x, int y, int r);
		bool 	movePiece(MovementType movement);
		void	putBlockInPlace();
		void	boardUpdate(SDL_Renderer* renderer, Game& game, bool isPlayer = 1);
		void	GameOver();
		void	Reset(Game& game);
		void    printBoard();
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

		//getters
		Tetromino* getPiece() { return currentPiece; }; 
		TetrisQueue* getQueue() { return queue; };
		std::vector<std::vector<int>>& getPlayingBoard() { return pBoard; };
		TetrisAI* getAI() { return ai; }
	private:
		std::vector<std::vector<int>> pBoard = std::vector<std::vector<int>>(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
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

		int		checkForLineClear(TSpinType type, SDL_Renderer* renderer, Game& game, bool shouldUpdateStats = 1);
		void	editCell(int i, int j, int val);
		TSpinType isTspin();
		bool	isCellOccupied(int x, int y);

		PreviousPieceState lastPieceState;
		PreviousPieceState ghostPiece;

		TetrisAI* ai = nullptr;
};
#endif // !TETRISCHALLENGE_H

