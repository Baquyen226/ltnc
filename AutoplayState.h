#pragma once
#ifndef AUTOPLAYSTATE_H
#define AUTOPLAYSTATE_H
#include <vector>

#include "game.h"
#include "texture.h"
#include "TetrisAI.h"

class IngameState;

class AutoplayState : public GameState {
public:
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;

	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;

	//unique function
	bool enter(Game& game, SDL_Renderer* renderer, MapData* _mapData);
	std::string getName() const override { return "AutoplayState"; }
private:
	int timer = 0;
	Uint64 enterTimestamp = 0;
	int noteIndex = 0; // Index of the current note being processed
	std::vector<Note> notes = {}; // Pointer to notes vector from IngameState
	TetrisAI* tetrisAI = nullptr;
	bool aiEnabled = false; // Flag to enable/disable AI
	IngameState* ingameWrapper = nullptr; // Wrapper for IngameState to handle Tetris and Rhythm parts
};
#endif
//save me claude