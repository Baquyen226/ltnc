#include "GameSettingState.h"
#include "constants.h"

bool GameSettingState::enter(Game& game, SDL_Renderer* renderer) {
	// Initialize back button
	backButton = new Button;
	backButton->InitWithText("Back", buttonColor, renderer, SCREEN_WIDTH / 2 - buttonWidth / 2, SCREEN_HEIGHT - buttonHeight - 50);
	backButton->setAlpha(globalAlpha);
	// Initialize sliders
	DASSlider = new Slider("DAS", DAS_SLIDER_X, DAS_SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT, 0, 400, 130);
	DASSlider->setAlpha(globalAlpha);
	DASSlider->Render(renderer);
	ARSlider = new Slider("ARR", ARR_SLIDER_X, ARR_SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT, 0, 200, 20);
	ARSlider->setAlpha(globalAlpha);
	ARSlider->Render(renderer);
	return true;
}

bool GameSettingState::exit() {
	return true;
}

void GameSettingState::handleEvent(Game& game, SDL_Event& event) {
	backButton->handleEvent(game, event);
	DASSlider->handleEvents(event);
	ARSlider->handleEvents(event);
}

void GameSettingState::update(Game& game, SDL_Renderer* renderer) {
	backButton->Update(game, renderer);
	if (backButton->Clicked()) {
		game.switchState(new MenuState, renderer);
		return;
	}
	DASSlider->Render(renderer);
	ARSlider->Render(renderer);
}

void GameSettingState::render(Game& game, SDL_Renderer* renderer) {
	// Render buttons and sliders
	backButton->setAlpha(globalAlpha);
	DASSlider->setAlpha(globalAlpha);
	ARSlider->setAlpha(globalAlpha);

	backButton->Render(game, renderer);
	DASSlider->Render(renderer);
	ARSlider->Render(renderer);
}

