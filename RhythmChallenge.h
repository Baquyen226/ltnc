#pragma once
#ifndef RHYTHMCHALLENGE_H
#define RHYTHMCHALLENGE_H

#include <vector>

#include "AudioManager.h"
#include "HelperDataType.h"
#include "GameState.h"
#include "texture.h"

struct JudgementFeedback {
	Text* textObject;
	Uint64 startTime;
	bool active;

	JudgementFeedback() : textObject(NULL), startTime(0), active(false) {}
};


class RhythmChallenge : public GameState {
public:
	//functions from parent class
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;

	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;

	//unique function
	bool enter(Game& game, SDL_Renderer* renderer, MapData* mapData);
	void showJudgement(Game& game, int t);
	void GameOver();
	void setAlpha(Uint8 a);
	void setState(CurrentIngameState state) { ThisIngameState = state; }

	CurrentIngameState ThisIngameState = NORMAL;
	Uint64 GameOverTimestamp = 0;
private:
	float alpha = 255.0f;
	std::vector<Note> notes = {};

	//this game stat
	Uint64 gameStart = 0;
	float MaxHealth = 100;
	float deltaHeath = 0.2f;
	float spawnDistance, NoteSpeed, BEAT_TRAVEL_TIME;
	float HPDrainRate = 0.1f;
	float HPDrainMultiplier = 1.0f;

	//asset
	AudioManager* harddrop = NULL;
	AudioManager* song = NULL;
	JudgementFeedback object;
	Text HP;
	Text ACCURACY;
	Text JUDGEMENT_PERFECT;
	Text JUDGEMENT_GOOD;
	Text JUDGEMENT_MISS;
};

#endif