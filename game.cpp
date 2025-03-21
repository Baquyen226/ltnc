#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>

#include "game.h"
#include "texture.h"
#include "constants.h"

Board* board = NULL;

void Game::Init() {
	//SDL stuff
	bool initialized = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (initialized) std::cerr << "SDL Initialized!" << "\n";
	else std::cerr << "Failed to initialize SDL: " << SDL_GetError << "\n";

	initialized = TTF_Init();
	if (initialized) std::cerr << "SDL_TTF Initialized!" << "\n";
	else std::cerr << "Failed to initialize SDL_TTF: " << SDL_GetError << "\n";

	initialized = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	if (initialized) std::cerr << "SDL_Mixer Initialized!" << "\n";
	else std::cerr << "Failed to initialize SDL_Mixer: " << SDL_GetError << "\n";

	int channel = Mix_AllocateChannels(32);
	std::cerr << channel << " channels allocated\n";
	std::cerr << "Actual allocated channels: " << Mix_AllocateChannels(-1) << "\n";

	SDL_AudioSpec spec;
	spec.freq = MIX_DEFAULT_FREQUENCY;
	spec.format = MIX_DEFAULT_FORMAT;
	spec.channels = MIX_DEFAULT_CHANNELS;

	if (!Mix_OpenAudio(0, &spec)) {
		std::cerr << "Failed to open audio: " << SDL_GetError() << "\n";
		return;
	}


	window = SDL_CreateWindow(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window != NULL) std::cerr << "Window created!" << "\n";
	else std::cerr << "An error occcured while trying to create window: " << SDL_GetError << "\n";

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer != NULL) std::cerr << "Renderer created!" << "\n";
	else std::cerr << "An error occcured while trying to create renderer: " << SDL_GetError << "\n";

	GAME_STATE = MENU;

	menu = new Menu();
	board = new Board();
	isRunning = true;
}

void Game::HandleEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if (event.type == SDL_EVENT_QUIT) {
			isRunning = false;
		}
		//messy as ???? imagine having multiple ingame state lol surely not right???? <- clueless
		switch (GAME_STATE) {
		case MENU:
			handleMenu(event);
			break;
		case INGAME:
			handleGame(event);
			break;
		case CONFIG:
			handleConfig(event);
			break;
		}
	}
}

void Game::Update() {
	Uint32 now = SDL_GetTicks();

	if (GAME_STATE == MENU) {
		// Handle menu
	}
	else if (GAME_STATE == INGAME) {
		// Handle game
		if (isRightHeld) {
			if (now - rightPressed > DELAYED_AUTO_SHIFT) {
				int p = (now - rightPressed) % REPEAT_RATE;
				if (p == 0) board->moveRight();
			}
		}

		if (isLeftHeld) {
			if (now - leftPressed > DELAYED_AUTO_SHIFT) {
				int p = (now - leftPressed) % REPEAT_RATE;
				if (p == 0) board->moveLeft();
			}
		}

		if (isDownHeld) {
			board->SoftDrop();
		}
		board->boardUpdate();
	}
	else if (GAME_STATE == CONFIG) {
		// Handle config
	}
	

}

void Game::Render() {
	SDL_RenderClear(renderer);
	if (GAME_STATE == MENU) {
		// Render menu
		menu->Render(renderer);
	}
	else if (GAME_STATE == INGAME) {
		// Render game
		board->Render(renderer);
	}
	else if (GAME_STATE == CONFIG) {
		// Render config
	}

	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
	if (board != NULL) {
		board->Clean();
		delete board;
	}
	delete menu;
	board = NULL;
	menu = NULL;
	renderer = NULL;
	window = NULL;
	std::cerr << "Game exited!" << "\n";
}