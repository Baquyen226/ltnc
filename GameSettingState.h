#pragma once
#include "game.h"
#include "GameState.h"
#include "Button.h"
#include "Slider.h"


//variables: DAS, ARR
class GameSettingState : public GameState {
public:
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;
	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;
	void setAlpha(Uint8 a) { globalAlpha = a; }
private:
	SDL_Color buttonColor = { 255, 255, 255, 255 };
	SDL_Color buttonHoverColor = { 96, 0, 0, 255 };
	SDL_Color buttonClickedColor = { 0, 0, 0, 255 };
	int buttonWidth = 200;
	int buttonHeight = 50;
	int buttonSpacing = 10;
	Texture background;
	SDL_Rect backgroundRect;
	float alpha = 255.0f;
	Button* backButton = nullptr;
	Slider* DASSlider = nullptr;
	Slider* ARSlider = nullptr;
	Uint8 globalAlpha = 255;
};
