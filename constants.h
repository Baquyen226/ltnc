#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL3/SDL.h>
#include <string>
#include <vector>

const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

static const char* WINDOW_TITLE	= "TITLE CARD"	;
const int SCREEN_WIDTH		= 1400			;
const int SCREEN_HEIGHT		= 900			;
const int BOARD_HEIGHT		= 22			;
const int BOARD_WIDTH		= 10			;
const int CELL_SIZE			= 30			;

//Board position(cell at 0,0)
const int BOARD_OFFSET_X	= SCREEN_WIDTH  / 2 - 200;
const int BOARD_OFFSET_Y	= SCREEN_HEIGHT / 2 - 300;
const int HOLD_PIECE_OFFSET_X = BOARD_OFFSET_X - CELL_SIZE * 7;
const int HOLD_PIECE_OFFSET_Y = BOARD_OFFSET_Y + CELL_SIZE * 2;
const int PIECE_PREVIEW					= 3;
const int PREVIEW_OFFSET_X				= BOARD_OFFSET_X + CELL_SIZE * 10 + SCREEN_WIDTH/20;
const int PREVIEW_OFFSET_Y				= BOARD_OFFSET_Y + CELL_SIZE * 2;


const int INITIAL_PIECE_POSITION_X		= 3;
const int INITIAL_PIECE_POSITION_Y		= 1;
const int INITIAL_ROTATION_STATE		= 0;

const int BASE_GRAVITY					= 90;
const int LINE_BETWEEN_LEVELS			= 10;
const int GRAVITY_INCREMENT				= 5;

const int DELAYED_AUTO_SHIFT			= 90; //6f equivalent
const int REPEAT_RATE					= 1;
const int SOFT_DROP_SPEED				= 1;



#endif
