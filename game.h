#pragma once
#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "HelperDataType.h"
#include "AudioManager.h"
#include "AllGameStates.h"
#include "constants.h"

class Game {
public:
	Game();
	~Game();

	void handleEvent();
	void update();
	void render();
	bool running() { return isRunning; };

	void switchState(GameState* newState, SDL_Renderer* renderer);
	void enterIngameStateWithMap(MapData* mapData);
	IngameStats* getStats() { return stats; };
	SDL_Renderer* getRenderer() { return renderer; };
	void Exit() { isRunning = false; };
private:
	bool isRunning = false;
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;
	GameState* currentState = NULL;
	IngameStats* stats = NULL;
};
#endif