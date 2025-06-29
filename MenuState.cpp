#include "MenuState.h"
#include "constants.h"
#include "game.h"
#include "texture.h"
#include "IngameState.h"
#include "AudioManager.h"

#include <iostream>

bool MenuState::enter(Game& game, SDL_Renderer* renderer) {
	// Load the background texture
	std::string backgroundPath = "assets/images/background.png";
	background.loadFromFile(renderer, backgroundPath);
	backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	background.setAlpha(255);
	// Create buttons
	Button* playButton = new Button;
	playButton->InitWithText("Play", buttonColor, renderer, SCREEN_WIDTH / 2 - buttonWidth / 2, SCREEN_HEIGHT / 2 - buttonHeight / 2);
	Button* exitButton = new Button;
	exitButton->InitWithText("Exit", buttonColor, renderer, SCREEN_WIDTH / 2 - buttonWidth / 2, SCREEN_HEIGHT / 2 + buttonHeight + buttonSpacing);
	buttons.push_back(playButton);
	buttons.push_back(exitButton);

	// Load the background music
	AudioManager* music = new AudioManager();
	std::string test = "menu.mp3";
	music->loadMusic(test);
	music->playMusic(-1); // Loop the music indefinitely
	return true;
}

bool MenuState::exit() {
	for (Button* button : buttons) {
		delete button;
		std::cerr << "Deleted buttons\n";
	}
	buttons.clear();
	Mix_FadeOutMusic(2000);
	return true;
}

void MenuState::handleEvent(Game& game, SDL_Event& event) {
	for (Button* button : buttons) {
		button->handleEvent(game, event);
	}
}

void MenuState::update(Game& game, SDL_Renderer* renderer) {
	for (Button* button : buttons) {
		button->Update(game, renderer);
		if (button->Clicked()) {
			if (button->getText() == "Play") {
				game.switchState(new LevelSelectorState, renderer);
				return;
			}
			else if (button->getText() == "Exit") {
				game.Exit();
				return;
			}
		}
	}
}

void MenuState::render(Game& game, SDL_Renderer* renderer) {
	// Render the background
	SDL_FRect destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	background.PartialRender(renderer, &destRect, &destRect);
	// Render buttons
	for (Button* button : buttons) {
		button->Render(game, renderer);
	}
}
