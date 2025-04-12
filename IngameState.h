#pragma once
#ifndef INGAMESTATE_H
#define INGAMESTATE_H
#include <vector>

#include "game.h"
#include "texture.h"
#include "IngameStats.h"
#include "GameState.h"
#include "HelperDataType.h"
#include "TetrisChallenge.h"
#include "RhythmChallenge.h"

class TetrisChallenge;
class RhythmChallenge;

class IngameState : public GameState {
public:
	//functions from parent class
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;

	void handleEvent(Game &game, SDL_Event &event) override;
	void update(Game &game, SDL_Renderer* renderer) override;
	void render(Game &game, SDL_Renderer* renderer) override;

	//unique function
	bool enter(Game& game, SDL_Renderer* renderer, MapData* _mapData);
private:
	TetrisChallenge* TetrisPart = NULL;
	RhythmChallenge* RhythmPart = NULL;
	float globalAlpha = 255.0f;
	Uint64 GameOverTimestamp;
	MapData* mapData = NULL;
};
#endif