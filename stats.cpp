#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <iostream>

#include "stats.h"
#include "constants.h"
#include "texture.h"

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
void GameStats::addLines(int lines) {
	totalLineCleared += lines;
	lineClearedThisLevel += lines;

}
void GameStats::increaseLevel() {
	level++;
	lineClearedThisLevel -= 10;
	gravity = BASE_GRAVITY - level * GRAVITY_INCREMENT;
	if (gravity < 0) gravity = 1;
}

void GameStats::Update(int clearedLines) {
	if (clearedLines > 0) {
		addPoints(clearedLines);
		addLines(clearedLines);
		if (lineClearedThisLevel > 10) increaseLevel();
	}
}

void GameStats::Render(SDL_Renderer* renderer) {
	SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };

	std::string s = "Points: " + std::to_string(points);
	toBoard->loadTextToTexture(s, COLOR_WHITE, renderer);
	toBoard->Render(150, 740, renderer, NULL);

	s = "Lines: " + std::to_string(totalLineCleared);
	toBoard->loadTextToTexture(s, COLOR_WHITE, renderer);
	toBoard->Render(150, 770, renderer, NULL);

	s = "Level:  " + std::to_string(level);
	toBoard->loadTextToTexture(s, COLOR_WHITE, renderer);
	toBoard->Render(150, 800, renderer, NULL);
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
