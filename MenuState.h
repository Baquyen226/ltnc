#pragma once
#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <vector>

class Button;

#include "Button.h"
#include "GameState.h"

class MenuState : public GameState {
public:
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;

	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;
private:
	std::vector<Button*> buttons;
	SDL_Color buttonColor = { 255, 255, 255, 255 };
	SDL_Color buttonHoverColor = { 0, 0, 0, 255 };
	SDL_Color buttonClickedColor = { 0, 0, 0, 255 };
	int buttonWidth = 200;
	int buttonHeight = 50;
	int buttonSpacing = 10;
	//background
	Texture background;
	SDL_Rect backgroundRect;
	float alpha = 255.0f;
};

#endif
