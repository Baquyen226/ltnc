#pragma once
#ifndef INGAMESTATS_H
#define INGAMESTATS_H

#include <SDL_ttf.h>

#include "constants.h"
#include "texture.h"
#include "HelperDataType.h"

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
    double		    points = 0;
    int             b2b = -1; //i hope this doesnt cause any unforeseen problems *insert laughtrack*
	Uint64		    lineClearStartTime = 0; // Time when the line clear effect starts
    bool 		    clearActive = false;
	bool 		    B2BActive   = false;
    Text* pointsText = nullptr;
    Text* levelText = nullptr; 
    Text* linesText = nullptr;
	Text* lineClearText = nullptr;
	Text* TSpinText = nullptr;
	Text* B2BText = nullptr; // Back-to-back text, not used yet

    Text CLEAR_SINGLE;
	Text CLEAR_DOUBLE;
	Text CLEAR_TRIPLE;
	Text CLEAR_QUAD;
	Text T_SPIN;
	Text T_SPIN_MINI;
    Text NOTHINGLOL;
	void getLineClearText(int clearedLines, TSpinType type);

	void loadAssets(SDL_Renderer* renderer);
    void addPoints(int clearedLines, TSpinType type);
    void addLines(int clearedlines);
    void increaseLevel();
    void Update(int clearedLines, TSpinType type, SDL_Renderer* renderer);
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
	const bool isHealthEmpty() { return ActualHealth <= 0.1f; };


    //getters
    int getLevel();
    int getTotalLinesCleared();
    int getPoints();

	//setters
	void setAlpha(Uint8 a) { if (pointsText) pointsText->setAlpha(a); if (levelText) levelText->setAlpha(a); if (linesText) linesText->setAlpha(a); alpha = a; };
};
#endif
