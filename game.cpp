#include "SDL.h"
#include <iostream>

#include "game.h"
#include "board.h"


Board* board = new Board();

void Game::Init() {
	bool initialized = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	if (initialized) std::cerr << "SDL Initialized!" << std::endl;

	window = SDL_CreateWindow(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window != NULL) std::cerr << "Window created!" << std::endl;
	else std::cerr << "An error occcured: " << SDL_GetError << std::endl;

	renderer = SDL_CreateRenderer(window, NULL);
	if (window != NULL) std::cerr << "Renderer created!" << std::endl;
	else std::cerr << "An error occcured: " << SDL_GetError << std::endl;

	isRunning = true;
}

void Game::HandleEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch (event.type) {
		case SDL_EVENT_QUIT:
			isRunning = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (board->isGameGoing()) {
				if (event.key.key == SDLK_RIGHT) board->moveRight();
				if (event.key.key == SDLK_LEFT) board->moveLeft();
				if (event.key.key == SDLK_X && !event.key.repeat /*Not a repeat */) board->Rotate();
				if (event.key.key == SDLK_Z && !event.key.repeat /*Not a repeat */) board->ReversedRotate();
				if (event.key.key == SDLK_SPACE) board->HardDrop();
				if (event.key.key == SDLK_DOWN) board->SoftDrop();
			}

			break;
		default:
			break;
		}
	}
}

void Game::Update() {
	board->boardUpdate();
}

void Game::Render() {
	SDL_RenderClear(renderer);
	board->Render(renderer);

	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cerr << "Game exited!" << std::endl;
}