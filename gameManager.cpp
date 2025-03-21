#include <iostream>

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


std::string move = "asset/sound/move.wav";
std::string rotate = "asset/sound/rotate.wav";
std::string harddrop = "asset/sound/harddrop.wav";


void Game::handleGame(SDL_Event &event) {
	switch (event.type) {
	case SDL_EVENT_QUIT:
		isRunning = false;
		break;
	case SDL_EVENT_KEY_DOWN:
		if (board->isGameGoing()) {
			if (event.key.key == SDLK_RIGHT) {
				isRightHeld = true;
				rightPressed = SDL_GetTicks();
				board->moveRight();
				sfx.playSound(move, PIECE_MOVING);
			}
			if (event.key.key == SDLK_LEFT) {
				isLeftHeld = true;
				leftPressed = SDL_GetTicks();
				board->moveLeft();
				sfx.playSound(move, PIECE_MOVING);
			}
			if (event.key.key == SDLK_DOWN) {
				isDownHeld = true;
				board->SoftDrop();
				sfx.playSound(move, PIECE_MOVING);
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
		}
		break;
	case SDL_EVENT_KEY_UP:
		if (board->isGameGoing()) {
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
	default:
		break;
	}
}

void Game::handleConfig(SDL_Event& event) {
	switch (event.type) {
	case SDL_EVENT_KEY_DOWN:
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
		break;
	}
}