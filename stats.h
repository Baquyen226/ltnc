#pragma once
#ifndef STATS_H
#define STATS_H
#include <SDL_ttf.h>

#include "constants.h"
#include "texture.h"

struct GameStats {
	int			    level = 0;
	int			    totalLineCleared = 0;
	int			    lineClearedThisLevel = 0;
	int			    gravity = BASE_GRAVITY;
	int			    points = 0;
	int			    reward[4] = { 40, 120, 300, 1200 };
    TextureManager* toBoard = NULL;
    TTF_Font* font = NULL;

    GameStats();
    void Reset();

    void addPoints(int clearedLines);
    void addLines(int clearedlines);
    void increaseLevel();
    void Update(int clearedLines);
    void Render(SDL_Renderer* renderer);

    int getLevel();
    int getTotalLinesCleared();
    int getPoints();
};

#endif