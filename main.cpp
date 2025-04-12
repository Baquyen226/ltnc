#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h> 

#include "game.h"
#include "MapDataLoader.h"

int main() {
	TTF_Init(); //too lazy to actually think of a viable solution soz dfjhsdoghdso
	Game* game = new Game();
	Uint64 now, deltaTime;
	while (game->running()) {
		now = SDL_GetTicks();
		game->handleEvent();
		game->update();
		game->render();
		deltaTime = SDL_GetTicks() - now;
		if (deltaTime < 16) SDL_Delay(16 - deltaTime);
	}
	game->~Game();
	std::cerr << "Game exited\n";
	return 0;
}