#pragma once
#ifndef LEVELCOMPLETESTATE_H
#define LEVELCOMPLETESTATE_H

#include <vector>

#include "Button.h"
#include "game.h"
#include "texture.h"

class LevelCompleteState : public GameState {
public:
	//functions from parent class
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;
	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;
private:
	Texture background;
	SDL_Rect backgroundRect;
	float alpha = 255.0f;
	//buttons
	std::vector<Button*> buttons;
	SDL_Color buttonColor = { 255, 255, 255, 255 };
	SDL_Color buttonHoverColor = { 0, 0, 0, 255 };
	SDL_Color buttonClickedColor = { 0, 0, 0, 255 };
	int buttonWidth = 200;
	int buttonHeight = 50;
	int buttonSpacing = 200;
	//text
	std::vector<Text*> texts;
};

#endif // !

