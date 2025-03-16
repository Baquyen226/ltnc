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

class Board {
public:
	GameStats	stats;

	Board();
	void	Clean();
	int**	getPlayingField();
	void	clearBoard();
	void	printBoard();
	void	getRenderColor(SDL_Renderer* renderer, int piece_id);
	void	Render(SDL_Renderer* renderer);
	void	editCell(int i, int j, int val);
	bool	canMove(int x, int y, int r);
	void	moveRight();
	void	moveLeft();
	void	Rotate();
	void	ReversedRotate();
	void	HardDrop();
	void	SoftDrop();
	void	deleteOldBlock();
	void	putBlockInPlace();
	int		checkForLineClear();
	void	boardUpdate();
	void	GameOver();
	bool	isGameGoing();
	void	fallDown();	
private:
	int			pBoard[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
	int			fTimer	;
	int			level	;
	bool		isGameOver = false;

	Queue*		queue = NULL;
	Tetromino* currentPiece = NULL;
};


#endif