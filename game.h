#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "board.h"
#include "menu.h"
#include "configScreen.h"
#include "sfx.h"

enum GameState {
	MENU,
	INGAME,
	CONFIG
};

class Game {
public:
	GameState GAME_STATE = MENU;
	Menu* menu = NULL;

	void Init();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
	void setGameControl(int DAS, int RR);
	bool Running() { return isRunning; };
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	ConfigScreen configScreen;

	//keypress stuff
	Uint32 leftPressed;
	Uint32 rightPressed;
	Uint32 timeHeldLeft;
	Uint32 timeHeldRight;
	bool isRightHeld = false;
	bool isLeftHeld = false;
	bool isDownHeld = false;

	Board* board = NULL;

	//handle game state, function is in gameManager.cpp
	void handleMenu(SDL_Event& event);
	void handleGame(SDL_Event& event);
	void handleConfig(SDL_Event& event);

	//control
	int DELAYED_AUTO_SHIFT = 150;
	int REPEAT_RATE = 22;

};

#endif