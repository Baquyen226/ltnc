#include "TetrisChallenge.h"
#include "game.h"

bool TetrisChallenge::enter(Game& game, SDL_Renderer* renderer) {
	board = new TetrisBoard;
	board->clearBoard();
	return true;
}

bool TetrisChallenge::exit() {
	delete board;
	board = NULL;
	return true;
}

void TetrisChallenge::handleEvent(Game& game, SDL_Event& event) {
	switch (event.type) {
	case SDL_EVENT_KEY_DOWN:
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
			}

			if (event.key.key == SDLK_X && !event.key.repeat /*Not a repeat */) {
				board->Rotate();
			}

			if (event.key.key == SDLK_Z && !event.key.repeat /*Not a repeat */) {
				board->ReversedRotate();
			}

			if (event.key.key == SDLK_A && !event.key.repeat /*Not a repeat */) {
				board->Rotate180();
			}

			if (event.key.key == SDLK_C && !event.key.repeat /*Not a repeat */) {
				board->HoldPiece();
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
	}
}

void TetrisChallenge::update(Game& game, SDL_Renderer* renderer) {
	if (board->state != NORMAL) return;
	game.getStats()->setAlpha(alpha);

	Uint64 now = SDL_GetTicks();
	if (isRightHeld) {
		if (now - rightPressed > DELAYED_AUTO_SHIFT) {
			int p = (now - rightPressed - DELAYED_AUTO_SHIFT) % REPEAT_RATE;
			if (p == 0) board->moveRight();
		}
	}

	if (isLeftHeld) {
		if (now - leftPressed > DELAYED_AUTO_SHIFT) {
			int p = (now - leftPressed - DELAYED_AUTO_SHIFT) % REPEAT_RATE;
			if (p == 0) board->moveLeft();
		}
	}

	if (isDownHeld) {
		board->SoftDrop();
	}
	board->boardUpdate(renderer, game);
}

void TetrisChallenge::render(Game& game, SDL_Renderer* renderer) {
	board->Render(renderer);
}

void TetrisChallenge::setAlpha(Uint8 a) {
	if (board == NULL) return;
	board->setAlpha(a);
	alpha = a;
}
