#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#include "game.h"
#include "constants.h"

Game* game = NULL;

int main() {
	game = new Game();

	game->Init();

	while (game->Running()) {
		Uint32 frameStart = SDL_GetTicks();
		int frameTime;

		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY - frameTime); // Delay the remaining time
		}
	}

	game->Clean();
	return 0;
}