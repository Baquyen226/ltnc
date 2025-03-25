#include <iostream>
#include <sstream>

#include "game.h"


void Game::handleMenu(SDL_Event& event) {
	switch (event.type) {
		/*case SDL_EVENT_KEY_DOWN:
			switch (event.key.key) {
			case SDLK_UP:
				std::cerr << "UP" << std::endl;
				break;
			case SDLK_DOWN:
				std::cerr << "DOWN" << std::endl;
				break;
			case SDLK_RETURN:
				std::cerr << "RETURN" << std::endl;
				break;
			}
			break;*/
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		float x, y;
		SDL_GetMouseState(&x, &y);
		int i = menu->handleClicks(x, y);
		if (i == 0) {
			GAME_STATE = INGAME;
			board = new Board();
			board->loadAsset(renderer);
			//load config
			std::ifstream file("config.txt");
			if (!file.is_open()) setGameControl(DEFAULT_DELAYED_AUTO_SHIFT, DEFAULT_REPEAT_RATE);

			float dasValue, repeatRateValue, volumeValue;
			file >> dasValue >> repeatRateValue >> volumeValue;

			setGameControl(dasValue, repeatRateValue);
			std::cerr << "Current game control : DAS: " << DELAYED_AUTO_SHIFT << " REPEAT_RATE: " << REPEAT_RATE << "\n";
		}
		else if (i == 1) {
			GAME_STATE = CONFIG;
		}
		else if (i == 2) {
			isRunning = false;
		}
		else {
			std::cerr << "Button not clicked\n";
		}
		break;
	}
}

std::string rotate = "asset/sound/rotate.wav";
std::string harddrop = "asset/sound/harddrop.wav";


void Game::handleGame(SDL_Event& event) {
	switch (event.type) {
	case SDL_EVENT_QUIT:
		isRunning = false;
		break;
	case SDL_EVENT_KEY_DOWN:
		switch (board->state) {
		case NORMAL:
			if (event.key.key == SDLK_RIGHT) {
				isRightHeld = true;
				rightPressed = SDL_GetTicks();
				board->moveRight();
			}

			if (event.key.key == SDLK_LEFT) {
				isLeftHeld = true;
				leftPressed = SDL_GetTicks();
				board->moveLeft();
			}

			if (event.key.key == SDLK_DOWN) {
				isDownHeld = true;
				board->SoftDrop();
			}

			if (event.key.key == SDLK_SPACE && !event.key.repeat /*Not a repeat */) {
				board->HardDrop();
				sfx.playSound(harddrop, PIECE_ROTATE);
			}

			if (event.key.key == SDLK_X && !event.key.repeat /*Not a repeat */) {
				board->Rotate();
				sfx.playSound(rotate, PIECE_ROTATE);
			}

			if (event.key.key == SDLK_Z && !event.key.repeat /*Not a repeat */) {
				board->ReversedRotate();
				sfx.playSound(rotate, PIECE_ROTATE);
			}

			if (event.key.key == SDLK_A && !event.key.repeat /*Not a repeat */) {
				board->Rotate180();
				sfx.playSound(rotate, PIECE_ROTATE);
			}

			if (event.key.key == SDLK_C && !event.key.repeat /*Not a repeat */) {
				board->HoldPiece();
			}

			if (event.key.key == SDLK_ESCAPE && !event.key.repeat) {
				board->state = PAUSED;
				std::cerr << "Game paused\n";
			}
			break;

		case GAMEOVER:
			if (event.key.key == SDLK_R && !event.key.repeat) {
				board->Reset();
			}
			break;

		case PAUSED:
			if (event.key.key == SDLK_ESCAPE && !event.key.repeat) {
				board->state = NORMAL;
				std::cerr << "Game resumed!\n";
			}
			break;
		}
		break;
	case SDL_EVENT_KEY_UP:
		if (board->state == NORMAL) {
			if (event.key.key == SDLK_RIGHT) {
				isRightHeld = false;
			}
			if (event.key.key == SDLK_LEFT) {
				isLeftHeld = false;
			}
			if (event.key.key == SDLK_DOWN) {
				isDownHeld = false;
			}
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (board->state == PAUSED || board->state == GAMEOVER) {
			float x, y;
			SDL_GetMouseState(&x, &y);
			if (board->MENU_BUTTON.isClicked(x, y)) GAME_STATE = MENU;
			if (board->RETRY_BUTTON.isClicked(x, y)) board->Reset();
		}
		break;
	default:
		break;
	}

}

void Game::handleConfig(SDL_Event& event) {
	switch (event.type) {
	/*case SDL_EVENT_KEY_DOWN:
		switch (event.key.key) {
		case SDLK_UP:
			std::cerr << "UP" << std::endl;
			break;
		case SDLK_DOWN:
			std::cerr << "DOWN" << std::endl;
			break;
		case SDLK_RETURN:
			std::cerr << "RETURN" << std::endl;
			break;
		}
		break;*/
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		float x, y;
		SDL_GetMouseState(&x, &y);
		if (configScreen.BACK.isClicked(x, y)) GAME_STATE = MENU;
		if (configScreen.SAVE.isClicked(x, y)) {
			configScreen.saveConfig();
			std::cerr << "Saved config!\n";
		}
		break;
	}

	configScreen.handleEvents(event);
}