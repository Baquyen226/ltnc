#pragma once
#ifndef TETRISCHALLENGE_H
#define TETRISCHALLENGE_H

#include "GameState.h"
#include "TetrisBoard.h"
#include "HelperDataType.h"

class TetrisChallenge : public GameState {
public:
	//override shits
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;

	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;

	//unique functions
	CurrentIngameState getState() { return board->state; };
	Uint64 getGameOverTimestamp() { return board->GameOverTimestamp; };
	void setAlpha(Uint8 v);
	void setState(CurrentIngameState s) { board->state = s; };
private:
	Uint64 leftPressed;
	Uint64 rightPressed;
	Uint64 timeHeldLeft;
	Uint64 timeHeldRight;
	bool isRightHeld = false;
	bool isLeftHeld = false;
	bool isDownHeld = false;
	float alpha = 255.0f;

	TetrisBoard* board = nullptr;
	int DELAYED_AUTO_SHIFT = 90;
	int REPEAT_RATE = 1;
};

#endif
