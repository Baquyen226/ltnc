#pragma once
#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H
class Text;
class game;
class Button;

#include "game.h"
#include "texture.h"
#include "Button.h"
#include <vector>

class GameOverState : public GameState {
public:
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;

	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;

	bool enter(Game& game, SDL_Renderer* renderer, MapData* mapData);
	void setAlpha(Uint8 a);
private:
	Text GAME_OVER;
	float alpha = 255.0f;
	MapData* mapData = NULL;
	std::vector<Button*> buttons;
};

#endif