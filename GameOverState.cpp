#include "GameOverState.h"
#include "constants.h"

bool GameOverState::enter(Game& game, SDL_Renderer* renderer) {
	std::cerr << "Entering GameOver state\n";
	GAME_OVER.loadFont("assets/fonts/8-bit.ttf", 80);
	GAME_OVER.loadTextToTexture("GAME OVER", { 255, 255, 255, 255 }, renderer);

	Button* restartButton = new Button;
	Button* menuButton = new Button;
	restartButton->InitWithText("Restart", { 255, 255, 255, 255 }, renderer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50);
	restartButton->setAlpha(255);
	buttons.push_back(restartButton);
	menuButton = new Button;
	menuButton->InitWithText("Menu", { 255, 255, 255, 255 }, renderer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100);
	menuButton->setAlpha(255);
	buttons.push_back(menuButton);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	return true; 
}

bool GameOverState::enter(Game& game, SDL_Renderer* renderer, MapData* _mapData) {
	std::cerr << "Entering GameOver state with mapData at this address:" << _mapData << "\n";
	GAME_OVER.loadFont("assets/fonts/8-bit.ttf", 80);
	GAME_OVER.loadTextToTexture("GAME OVER", { 255, 255, 255, 255 }, renderer);

	Button* restartButton = new Button;
	Button* menuButton = new Button;
	restartButton->InitWithText("Restart", { 255, 255, 255, 255 }, renderer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50);
	restartButton->setAlpha(255);
	buttons.push_back(restartButton);
	menuButton = new Button;
	menuButton->InitWithText("Menu", { 255, 255, 255, 255 }, renderer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100);
	menuButton->setAlpha(255);
	buttons.push_back(menuButton);

	mapData = _mapData;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	return true;
}

bool GameOverState::exit() { 
	for (auto button : buttons) {
		delete button;
		std::cerr << "Deleted buttons\n";
	}
	buttons.clear();
	return true; 
}

void GameOverState::handleEvent(Game& game, SDL_Event& event) {
	for (auto button : buttons) {
		button->handleEvent(game, event);
		if (button->Clicked()) {
			if (button->getText() == "Restart") {
				if (mapData) {
					//std::cerr << "Restarting game with this map data:" << mapData->directoryName <<"\n";
					game.enterIngameStateWithMap(mapData); //mem leak les goo
					return;
				}
				else {
					/*std::cerr << "MapData is null, rolling back to the default map\n";*/
					game.switchState(new IngameState, game.getRenderer());
					return;
				}
			}
			else if (button->getText() == "Menu") {
				if (mapData) {
					std::cerr << "Deleting mapData\n";
					delete mapData;
					mapData = nullptr;
				}
				game.switchState(new MenuState, game.getRenderer());
				return;
			}
		}
	}
}

void GameOverState::update(Game &game, SDL_Renderer* renderer) {}

void GameOverState::render(Game& game, SDL_Renderer* renderer) {
	GAME_OVER.Render(renderer, SCREEN_WIDTH / 2 - 230, SCREEN_HEIGHT / 2 - 230);
	for (auto button : buttons) {
		button->Render(game, renderer);
	}
}
