#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL3/SDL.h>
#include <string>
#include <vector>

const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

static const char* WINDOW_TITLE	= "YET ANOTHER TETRIS"	;
const int SCREEN_WIDTH		= 1300			;
const int SCREEN_HEIGHT		= 900			;
const int BOARD_HEIGHT		= 22			;
const int BOARD_WIDTH		= 10			;
const int CELL_SIZE			= 30			;

//Menu position
const int TITLE_CARD_X		= SCREEN_WIDTH  / 2 - 150;
const int TITLE_CARD_Y		= SCREEN_HEIGHT / 3 - 200;

const int BUTTON_START_X	= SCREEN_WIDTH / 2 - 140;
const int BUTTON_START_Y	= SCREEN_HEIGHT / 2 - 100;
const int BUTTON_SIZE_X		= 280;
const int BUTTON_SIZE_Y		= 90;


const int BUTTON_CONFIG_X	= SCREEN_WIDTH / 2 - 140;
const int BUTTON_CONFIG_Y	= SCREEN_HEIGHT / 2;

const int BUTTON_QUIT_X		= SCREEN_WIDTH / 2 - 140;
const int BUTTON_QUIT_Y		= SCREEN_HEIGHT / 2 + 100;


//Board position(cell at 0,0)
const int BOARD_OFFSET_X				= SCREEN_WIDTH  / 2 - 200;
const int BOARD_OFFSET_Y				= SCREEN_HEIGHT / 2 - 300;
const int HOLD_PIECE_OFFSET_X			= BOARD_OFFSET_X - CELL_SIZE * 7;
const int HOLD_PIECE_OFFSET_Y			= BOARD_OFFSET_Y + CELL_SIZE * 2;
const int PIECE_PREVIEW					= 3;
const int PREVIEW_OFFSET_X				= BOARD_OFFSET_X + CELL_SIZE * 10 + SCREEN_WIDTH/20;
const int PREVIEW_OFFSET_Y				= BOARD_OFFSET_Y + CELL_SIZE * 2;

const int INITIAL_PIECE_POSITION_X = 3;
const int INITIAL_PIECE_POSITION_Y = 1;
const int INITIAL_ROTATION_STATE = 0;

const int BASE_GRAVITY = 90;
const int LINE_BETWEEN_LEVELS = 10;
const int GRAVITY_INCREMENT = 5;

const int TEXT_GAMEOVER_POSX_CENTER		= BOARD_OFFSET_X + BOARD_WIDTH / 2 * CELL_SIZE;
const int TEXT_GAMEOVER_POSY_CENTER		= BOARD_OFFSET_Y + BOARD_HEIGHT / 4 * CELL_SIZE;

const int BUTTON_SPRITE_FACTOR			= 16;
const int RETRY_BUTTON_X				= 3;
const int RETRY_BUTTON_Y				= 0;
const int MENU_BUTTON_X					= 4;
const int MENU_BUTTON_Y					= 6;
const int RESUME_BUTTON_X				= 1;
const int RESUME_BUTTON_Y				= 3;
const int BUTTON_SPRITE_SIZE			= 48;

//Default configuration
const int DEFAULT_DELAYED_AUTO_SHIFT = 150; //~10f equivalent
const int DEFAULT_REPEAT_RATE = 22;

#endif
