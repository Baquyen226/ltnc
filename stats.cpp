#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>

#include "stats.h"
#include "sfx.h"


GameStats::GameStats() {
	level = 0;
	totalLineCleared = 0;
	lineClearedThisLevel = 0;
	points = 0;
	gravity = BASE_GRAVITY;
	toBoard = new TextureManager();
	toBoard->loadFont("asset/font/8-bit.ttf", 24);
}

void GameStats::addPoints(int clearedLines) {
	points += reward[clearedLines - 1] * (level + 1);
}

std::string line1 = "asset/sound/clearline.wav";
std::string line2 = "asset/sound/clearquad.wav";

void GameStats::addLines(int lines) {
	totalLineCleared += lines;
	lineClearedThisLevel += lines;
	if (lines == 4) sfx.playSound(line2, LINE_CLEAR);
	else sfx.playSound(line1, LINE_CLEAR);
}

void GameStats::increaseLevel() {
	level++;
	lineClearedThisLevel -= 10;
	gravity = BASE_GRAVITY * pow(0.85, level + 1);
	if (gravity <= 0) gravity = 1;
	std::string levelup = "asset/sound/levelup.wav";
	sfx.playSound(levelup, GAME_EVENT);
}

void GameStats::Update(int clearedLines) {
	if (clearedLines > 0) {
		addPoints(clearedLines);
		addLines(clearedLines);
		if (lineClearedThisLevel >= 10) increaseLevel();
	}
}

void GameStats::Render(SDL_Renderer* renderer) {
	SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };

	std::string s = "Points: " + std::to_string(points);
	toBoard->loadTextToTexture(s, COLOR_WHITE, renderer);
	toBoard->Render(100, 670, renderer, NULL);

	s = "Lines: " + std::to_string(totalLineCleared);
	toBoard->loadTextToTexture(s, COLOR_WHITE, renderer);
	toBoard->Render(100, 700, renderer, NULL);

	s = "Level:  " + std::to_string(level);
	toBoard->loadTextToTexture(s, COLOR_WHITE, renderer);
	toBoard->Render(100, 730, renderer, NULL);
}

int GameStats::getLevel() {
	return level;
}

int GameStats::getTotalLinesCleared() {
	return totalLineCleared;
}

int GameStats::getPoints() {
	return points;
}
