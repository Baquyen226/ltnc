#include "LevelCompleteState.h"
#include "constants.h"
#include "game.h"

#include <string>

bool LevelCompleteState::enter(Game &game, SDL_Renderer* renderer) {
	std::cerr << "Entering LevelComplete state\n";

	// Load the background texture
	std::string backgroundPath = "assets/images/background.png";
	background.loadFromFile(renderer, backgroundPath);
	backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	background.setAlpha(255);
	// Create buttons
	Button* restartButton = new Button;
	restartButton->InitWithText("Restart", buttonColor, renderer, SCREEN_WIDTH / 3 - 100, SCREEN_HEIGHT - 100);
	Button* menuButton = new Button;
	menuButton->InitWithText("Menu", buttonColor, renderer, SCREEN_WIDTH * 2 / 3  - 100 + buttonSpacing, SCREEN_HEIGHT - 100);
	buttons.push_back(restartButton);
	buttons.push_back(menuButton);

	//Create texts
	Text* levelCompleteText = new Text;
	levelCompleteText->loadFont("assets/fonts/8-bit.ttf", 80);
	levelCompleteText->loadTextToTexture("Level Complete", { 255, 255, 255, 255 }, renderer);
	levelCompleteText->setAlpha(255);
	texts.push_back(levelCompleteText);

	Text* pointsText = new Text;
	pointsText->loadFont("assets/fonts/8-bit.ttf", 40);
	pointsText->loadTextToTexture("Points: " + std::to_string(game.getStats()->getPoints()), { 255, 255, 255, 255 }, renderer);
	pointsText->setAlpha(255);
	texts.push_back(pointsText);

	Text* linesText = new Text;
	linesText->loadFont("assets/fonts/8-bit.ttf", 40);
	linesText->loadTextToTexture("Lines: " + std::to_string(game.getStats()->getTotalLinesCleared()), { 255, 255, 255, 255 }, renderer);
	linesText->setAlpha(255);
	texts.push_back(linesText);

	Text* comboText = new Text;
	comboText->loadFont("assets/fonts/8-bit.ttf", 40);
	comboText->loadTextToTexture("Combo: " + std::to_string(game.getStats()->combo), { 255, 255, 255, 255 }, renderer);
	comboText->setAlpha(255);
	texts.push_back(comboText);

	Text* maxCombo = new Text;
	maxCombo->loadFont("assets/fonts/8-bit.ttf", 40);
	maxCombo->loadTextToTexture("Max Combo: " + std::to_string(game.getStats()->maxCombo), { 255, 255, 255, 255 }, renderer);
	maxCombo->setAlpha(255);
	texts.push_back(maxCombo);

	Text* accuracyText = new Text;
	accuracyText->loadFont("assets/fonts/8-bit.ttf", 40);
	std::string accuracyString = std::to_string(game.getStats()->accuracy);
	accuracyString = accuracyString.substr(0, accuracyString.find('.') + 3); // Limit to 2 decimal places // wow i should have thought of this
	accuracyText->loadTextToTexture("Accuracy: " + accuracyString + "%", { 255, 255, 255, 255 }, renderer);
	accuracyText->setAlpha(255);
	texts.push_back(accuracyText);

	return true;
}

bool LevelCompleteState::exit() {
	for (Button* button : buttons) {
		delete button;
		std::cerr << "Deleted buttons\n";
	}
	buttons.clear();
	for (Text* text : texts) {
		delete text;
		std::cerr << "Deleted texts\n";
	}
	texts.clear();
	return true;
}

void LevelCompleteState::handleEvent(Game& game, SDL_Event& event) {
	for (Button* button : buttons) {
		button->handleEvent(game, event);
		if (button->Clicked()) {
			if (button->getText() == "Restart") {
				game.switchState(new IngameState, game.getRenderer());
				return;
			}
			else if (button->getText() == "Menu") {
				game.switchState(new MenuState, game.getRenderer());
				return;
			}
		}
	}
}

void LevelCompleteState::update(Game& game, SDL_Renderer* renderer) {
	for (Button* button : buttons) {
		button->Update(game, renderer);
	}
}

void LevelCompleteState::render(Game& game, SDL_Renderer* renderer) {
	// Render the background
	SDL_FRect destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	background.PartialRender(renderer, &destRect, &destRect);
	// Render level complete text
	int textW, textH;
	texts[0]->getSize(&textW, &textH);
	texts[0]->Render(renderer, SCREEN_WIDTH / 2 - textW / 2, SCREEN_HEIGHT / 3 - textH / 2 - 100);

	// Render texts
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 0 && j == 0) continue; // Skip tLEVEL COMPLETE thing
			int textW, textH;
			texts[i * 3 + j]->getSize(&textW, &textH);
			int offset = 0;
			if (i == 0) offset = -textH / 2; //center the first column
			int textX = SCREEN_WIDTH / 3 - 100 + SCREEN_WIDTH / 3 * i - textW / 2 + 50;
			int textY = SCREEN_HEIGHT / 2 - 100 + j * 50 - textH /2 + offset;
			texts[i * 3 + j]->Render(renderer, textX, textY);
		}
	}
	// Render buttons
	for (Button* button : buttons) {
		button->Render(game, renderer);
	}
}