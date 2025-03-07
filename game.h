#pragma once
#ifndef GAME_H
#define GAME_H
#include <SDL3/SDL.h>

class Game {
public:
	void Init();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
	bool Running() { return isRunning; };
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

#endif