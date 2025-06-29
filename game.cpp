#include <iostream>

#include "game.h"
Game::Game() {
	//if (!init) std::cerr << "An error occured: " << SDL_GetError() << "\n";
	//init sdl library
	bool init = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	if (!init) std::cerr << "An error occured while trying to init SDL: " << SDL_GetError() << "\n";

	init = TTF_Init();
	if (!init) std::cerr << "An error occured while trying to init SDL_TTF: " << SDL_GetError() << "\n";
	
	SDL_AudioSpec spec;
	spec.freq = MIX_DEFAULT_FREQUENCY;
	spec.format = MIX_DEFAULT_FORMAT;
	spec.channels = MIX_DEFAULT_CHANNELS;
	init = Mix_OpenAudio(0, &spec);
	if (!init) std::cerr << "An error occured while trying to init SDL_Mixer: " << SDL_GetError() << "\n";


	//create window n renderer
	window = SDL_CreateWindow(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) std::cerr << "An error occured while trying to create window: " << SDL_GetError() << "\n";

	renderer = SDL_CreateRenderer(window, 0);
	if (!renderer) std::cerr << "An error occured while trying to create renderer: " << SDL_GetError() << "\n";

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	isRunning = true;
	switchState(new GameSettingState, renderer);
	stats = new IngameStats();
}

Game::~Game() {
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	if (currentState) delete currentState;
	currentState = nullptr;
	if (stats) delete stats;
	stats = nullptr;
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

void Game::handleEvent() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_EVENT_QUIT:
			isRunning = false;
			break;
		default:
			currentState->handleEvent(*this, e);
		}
	}
}

void Game::update() {
	Uint64 now = SDL_GetTicks();
	currentState->update(*this, renderer);
}

void Game::render() {
	SDL_RenderClear(renderer);
	currentState->render(*this, renderer);
	SDL_RenderPresent(renderer);
}

void Game::switchState(GameState* newState, SDL_Renderer* renderer) {
	if (currentState) {
		currentState->exit();
		delete currentState;
	}

	currentState = newState;
	if (!currentState->enter(*this, renderer)) {
		std::cerr << "Cannot switch game state\n";
		exit(-1);
	}
}

void Game::enterIngameStateWithMap(MapData* mapData) {
	//std::cerr << "Is this thing even printing(Game/EnterWithMap)?, at address " << mapData << "\n";
	//mapData->print();
	IngameState* ingame = new IngameState;
	currentState = ingame;

	//sure the mapdata doesnt get deleted but it creates a mem leak ill probably pass by value later
	if (!ingame->enter(*this, renderer, mapData)) {
		std::cerr << "Cannot switch game state\n";
		exit(-1);
	}
}