#pragma once
#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#include "constants.h"
#include "queue.h"
#include "tetromino.h"
#include "stats.h"
#include "texture.h"
#include "UI.h"
#include "background.h"

//Movement stuff
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

enum BoardState {
	NORMAL,
	PAUSED, 
	GAMEOVER
};

class Board {
public:
	GameStats	stats;
	BoardState	state = NORMAL;
	Button MENU_BUTTON, RETRY_BUTTON;
	std::vector<Background> BG;

	Board();
	void    loadAsset(SDL_Renderer* renderer);
	void	Clean();
	int**	getPlayingField();
	void	clearBoard();
	void	printBoard();
	void	getRenderColor(SDL_Renderer* renderer, int piece_id);
	void	Render(SDL_Renderer* renderer);
	void	editCell(int i, int j, int val);
	bool	canMove(int x, int y, int r);
	bool	tryRotation(int newRotation, int direction);
	bool 	movePiece(MovementType movement);
	void	deleteOldBlock();
	void	putBlockInPlace();
	int		checkForLineClear();
	void	boardUpdate();
	void	GameOver();
	void	Reset();

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
private:
	int			pBoard[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
	int			fTimer;
	int			level;
	int 		holdPiece = -1;
	bool		isHoldUsed = false;

	Queue* queue = NULL;
	Tetromino* currentPiece = NULL;
	TextureManager state_box;
};


#endif