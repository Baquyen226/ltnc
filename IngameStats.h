#pragma once
#ifndef INGAMESTATS_H
#define INGAMESTATS_H

#include <SDL_ttf.h>

#include "constants.h"
#include "texture.h"

struct IngameStats {
    IngameStats();
    void Reset();
    float alpha = 255.0f;
	bool textLoaded = false;

    //Tetris
    int			    level = 0;
    int			    totalLineCleared = 0;
    int			    lineClearedThisLevel = 0;
    int			    gravity = BASE_GRAVITY;
    int			    points = 0;
    int			    line_clear_points_reward[4] = { 40, 120, 300, 1200 };
	int             line_clear_health_reward[4] = { 1, 2, 4, 8 };
    Text* pointsText = nullptr;
    Text* levelText = nullptr; 
    Text* linesText = nullptr;

    void addPoints(int clearedLines);
    void addLines(int clearedlines);
    void increaseLevel();
    void Update(int clearedLines, SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);

    //Rhythm
    float CurrentHealth = 100;
    float ActualHealth = 100;
    float MaxHealth = 100;
    float deltaHeath = 0.2f;
	float accuracy = 0.0f;
    float spawnDistance, NoteSpeed, BEAT_TRAVEL_TIME;
    int combo = 0, totalHits = 0, perfectHits = 0, goodHits = 0, maxCombo = 0;
    
    void registerHits(int judgement);
	bool isHealthEmpty() { return ActualHealth <= 0.1f; };


    //getters
    int getLevel();
    int getTotalLinesCleared();
    int getPoints();

	//setters
	void setAlpha(Uint8 a) { if (pointsText) pointsText->setAlpha(a); if (levelText) levelText->setAlpha(a); if (linesText) linesText->setAlpha(a); alpha = a; };
};
#endif
