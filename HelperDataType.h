#pragma once
#ifndef HELPERDATATYPE_H
#define HELPERDATATYPE_H

#include <vector>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include "texture.h"

enum CurrentIngameState {
	NORMAL,
	GAMEOVER,
	COMPLETE
};

struct Note {
	Uint64 timestamp;
	bool hit = false;
	bool missed = false;
	Uint64 fadeStartTime = 0; // Time when fade starts
	Uint64 hitPosition = 0;
};

struct MapData {
	std::string mapName;
	std::string artist;
	std::string audioFilePath;
	// Background image should be 1300x900px. If not it will be cropped, not the best idea but meh
	std::string backgroundFilePath;
	// Should be 500x300px. If not it will be cropped, not the best idea but meh
	// If not set, a default thumbnail will be used
	std::string thumbnailFilePath;
	std::string videoFilePath = ""; // Optional, can be empty
	int duration;

	int hp_drain_multiplier;
	double rating;
	std::vector<int> noteTimestamps;
	std::vector<int> events; //could be a pair not dealing with this yet

	std::string directoryName;
	MapData();
	void print();
};

enum EventType {
	LEVEL_CHANGE,
	HP_DRAIN_RATE_CHANGE,
};

struct VideoFrame {
	AVFrame* frame;
	int64_t pts;
};

enum MovementType {
	MOVE_LEFT,
	MOVE_RIGHT,
	ROTATE_CW,
	ROTATE_CCW,
	ROTATE_180,
	SOFT_DROP,
	HARD_DROP,
	FALSE_HARD_DROP, //for ghost piece
	MOVE_NONE
};

enum TSpinType {
	NONE,
	T_SPIN,
	T_SPIN_MINI
};

//The face at which the T-piece corners face relative to the top corner of the matrix
// {x, y} coordinates
static short frontCorner[4][2][2] = {
	{ { 0, 0 }, { 2, 0 } }, // Rotation 0
	{ { 2, 0 }, { 2, 2 } }, // Rotation 1
	{ { 0, 2 }, { 2, 2 } }, // Rotation 2
	{ { 0, 0 }, { 0, 2 } }  // Rotation 3
};



#endif
