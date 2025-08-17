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
	const bool running() { return isRunning; };

	void switchState(GameState* newState, SDL_Renderer* renderer);
	void enterIngameStateWithMap(MapData* mapData);
	void enterAutoplayStateWithMap(MapData* m);
	IngameStats* getStats();
	SDL_Renderer* getRenderer() { return renderer; };
	void Exit() { isRunning = false; };
private:
	bool isRunning = false;
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;
	GameState* currentState = nullptr;
	IngameStats* stats = nullptr;
};
#endif