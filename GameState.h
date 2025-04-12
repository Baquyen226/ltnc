#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SDL.h>
class Game;

class GameState {
public:
	virtual bool enter(Game& game, SDL_Renderer* renderer) = 0;
	virtual bool exit() = 0;

	virtual void handleEvent(Game& game, SDL_Event& event) = 0;
	virtual void update(Game& game, SDL_Renderer* renderer) = 0;
	virtual void render(Game& game, SDL_Renderer* renderer) = 0;

	virtual ~GameState() = default;
};


#endif