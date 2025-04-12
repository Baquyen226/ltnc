#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <iostream>

static const char* TITLE = "My test";
const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 900;

// Bar and hit constants
// timing
const Uint64 HIT_WINDOW = 700;

//render
const Uint64 BAR_X = SCREEN_WIDTH / 4;
const Uint64 BAR_Y = SCREEN_HEIGHT / 10;
const Uint64 BAR_WIDTH = 600;
const Uint64 BAR_HEIGHT = 20;

const Uint64 HIT_ZONE_W = 140;
const Uint64 PERFECT_HIT_ZONE_W = 60;
const Uint64 JUDGEMENT_X = BAR_X + 200;
const Uint64 JUDGEMENT_Y = BAR_Y + 100;

const Uint64 HEALTH_BAR_X = 50;
const Uint64 HEALTH_BAR_Y = 10;
const Uint64 HEALTH_BAR_W = 300;
const Uint64 HEALTH_BAR_H = 30;

const Uint64 FADE_OUT_TIME = 100;
const Uint64 FEEDBACK_DURATION = 400;
const Uint64 START_X = BAR_X + BAR_WIDTH;
const Uint64 HIT_ZONE_X = SCREEN_WIDTH / 4 + BAR_WIDTH / 2 - 70;
const Uint64 PERFECT_HIT_ZONE_X = HIT_ZONE_X + 40;

const Uint16 ACCURACY_X = SCREEN_WIDTH - 200;
const Uint16 ACCURACY_Y = 100;


// Tetris constants
const Uint8 BOARD_HEIGHT = 22;
const Uint8 BOARD_WIDTH = 10;
const Uint8 CELL_SIZE = 30;

const int BOARD_OFFSET_X = SCREEN_WIDTH / 2 - 200;
const int BOARD_OFFSET_Y = SCREEN_HEIGHT / 2 - 300;
const int HOLD_PIECE_OFFSET_X = BOARD_OFFSET_X - CELL_SIZE * 7;
const int HOLD_PIECE_OFFSET_Y = BOARD_OFFSET_Y + CELL_SIZE * 2;
const int PIECE_PREVIEW = 3;
const int PREVIEW_OFFSET_X = BOARD_OFFSET_X + CELL_SIZE * 10 + SCREEN_WIDTH / 20;
const int PREVIEW_OFFSET_Y = BOARD_OFFSET_Y + CELL_SIZE * 2;

const int INITIAL_PIECE_POSITION_X = 3;
const int INITIAL_PIECE_POSITION_Y = 1;
const int INITIAL_ROTATION_STATE = 0;
const int BASE_GRAVITY = 90;

// Map data loader constants
const std::string MAP_FOLDER_PATH = "assets/beatmap";
//const std::string MAP_PATTERN_SONG = R"(\[Song\]\r\nname = (.*)\r\nartist = (.*)\r\naudio = (.*)\r\nbackground = (.*)\r\nduration = (.*))";
//const std::string MAP_PATTERN_DIFFICULTY = R"(\[Difficulty\]\r\nhp_drain_multiplier = (.*)\r\nrating = (.*))";
//const std::string MAP_PATTERN_NOTES = R"(\[Notes\]\r\n(.*))";
//const std::string MAP_PATTERN_EVENTS = R"(\[Events\]\r\n(.*))";
//nope not doing this

const Uint32 MAP_LIST_X = SCREEN_WIDTH / 2 + 300;
const Uint32 MAP_LIST_Y = 100;
const Uint32 INDIVIDUAL_MAP_W = 600;
const Uint32 INDIVIDUAL_MAP_H = 140;
const Uint32 THUMBNAIL_SIZE_X = 136;
const Uint32 THUMBNAIL_SIZE_Y = 176;


//file reference
//[Song]
//name = Bad Apple!!
//artist = ZUN, Masayoshi Minoshima, Nomico
//audio = bad apple.mp4
//background = background.jpg
//duration = 216000
//
//[Difficulty]
//hp_drain_multiplier = 1
//rating = 3
//
//[Notes]
//some numbers
// 
//[Events]
//some things

#endif