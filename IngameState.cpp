#include <algorithm>

#include "IngameState.h"
#include "HelperDataType.h"

Uint64 elapsed = 0;

void drawBlackLowOpacityRect(SDL_Renderer* renderer, int x, int y, int w, int h, float alpha) {
    Uint8 _r, _g, _b, _a;
	SDL_GetRenderDrawColor(renderer, &_r, &_g, &_b, &_a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)alpha);
	SDL_FRect rect = { (float)x, (float)y, (float)w, (float)h };
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, _r, _g, _b, _a);
}

bool IngameState::enter(Game& game, SDL_Renderer* renderer) {
    TetrisPart = new TetrisChallenge;
    RhythmPart = new RhythmChallenge;
	if (!TetrisPart || !RhythmPart) return false;

	if (!TetrisPart->enter(game, renderer) || !RhythmPart->enter(game, renderer)) {
		std::cerr << "Failed to enter Tetris or Rhythm part\n";
		return false;
	}
	// Set the initial alpha value for both parts
	globalAlpha = 255.0f;
	TetrisPart->setAlpha(globalAlpha);
	RhythmPart->setAlpha(globalAlpha);
    TetrisPart->setState(NORMAL);
	RhythmPart->setState(NORMAL);
	game.getStats()->Reset();
    game.getStats()->textLoaded = false;
	// Initialize GameOverTimestamp
	GameOverTimestamp = 0;
	elapsed = 0;
	startTime = SDL_GetTicks(); // Initialize start time

	return TetrisPart->enter(game, renderer) && RhythmPart->enter(game, renderer);
}

bool IngameState::enter(Game& game, SDL_Renderer* renderer, MapData* MAPDATA) {
    //std::cerr << "Checking MapData when passed into IngameState::enter(), at address " << MAPDATA << "\n";
    //std::cerr << "Directory name while inside IngameState::enter(): " << MAPDATA->directoryName << "\n";
    //MAPDATA->print();
    TetrisPart = new TetrisChallenge;
    RhythmPart = new RhythmChallenge;
    mapData = MAPDATA;
    /*std::cerr << "Is this thing even printing(in IngameState::enter)?\n";
    mapData->print();*/
    if (!TetrisPart || !RhythmPart || !mapData) {
		std::cerr << "Failed to initialize Ingame State.\n";
        return false;
    }

    //init video
    if (mapData->videoFilePath != ""){
        std::string videoPath = mapData->directoryName + mapData->videoFilePath;
        videoPlayer = new VideoPlayer(videoPath.c_str(), renderer);
    }

    //init tetris and rhythm
    if (!TetrisPart->enter(game, renderer) || !RhythmPart->enter(game, renderer, mapData)) {
        std::cerr << "Failed to enter Tetris or Rhythm part\n";
        return false;
    }
    // Set the initial alpha value for both parts
    globalAlpha = 255.0f;
    TetrisPart->setAlpha(globalAlpha);
    RhythmPart->setAlpha(globalAlpha);
    TetrisPart->setState(NORMAL);
    RhythmPart->setState(NORMAL);
    game.getStats()->Reset();
    game.getStats()->textLoaded = false;
    // Initialize GameOverTimestamp
    GameOverTimestamp = 0;
    elapsed = 0;

	return true;
}

bool IngameState::exit() {
    if (videoPlayer != nullptr) {
        videoPlayer->stop();
        delete videoPlayer;
        videoPlayer = nullptr;
    }
	return TetrisPart->exit() && RhythmPart->exit();
}

void IngameState::handleEvent(Game& game, SDL_Event& event)  {
    if (TetrisPart->getState() == GAMEOVER || RhythmPart->ThisIngameState == GAMEOVER) return;
    TetrisPart->handleEvent(game, event);
    RhythmPart->handleEvent(game, event);
}

void IngameState::update(Game& game, SDL_Renderer* renderer) {
    if (!TetrisPart || !RhythmPart) return;

    //Hacky fix so that setAlpha doesnt fricking get called after board was deleted)
    if (elapsed >= 1950) {
        if (mapData == nullptr) {
            //std::cerr << "MapData is null, rolling back to the default map\n";
            game.switchState(new GameOverState, renderer);
            return;
        }
        else {
            //std::cerr << "MapData is not null, passing it to GameOverState\n";
            GameOverState* gameOverState = new GameOverState;
            MapData* m = mapData;
            mapData = nullptr;
            game.switchState(gameOverState, renderer);
            gameOverState->enter(game, renderer, m);
            return;
        }
    }

    if (TetrisPart->getState() == GAMEOVER || RhythmPart->ThisIngameState == GAMEOVER) {
        GameOverTimestamp = std::max(TetrisPart->getGameOverTimestamp(), RhythmPart->GameOverTimestamp);
        Uint64 fadeDuration = 2000; // 2 seconds fade-out
        elapsed = SDL_GetTicks() - GameOverTimestamp;
        globalAlpha = std::max(0, 255 - (int)(elapsed * 255 / fadeDuration));
        TetrisPart->setAlpha(globalAlpha);
        RhythmPart->setAlpha(globalAlpha);
        return;
    }

    if (RhythmPart->ThisIngameState == COMPLETE) {
        GameOverTimestamp = std::max(TetrisPart->getGameOverTimestamp(), RhythmPart->GameOverTimestamp);
        Uint64 fadeDuration = 2000; // 2 seconds fade-out
        elapsed = SDL_GetTicks() - GameOverTimestamp;
        globalAlpha = std::max(0, 255 - (int)(elapsed * 255 / fadeDuration));
        if (elapsed > fadeDuration) game.switchState(new LevelCompleteState, renderer);
        if (elapsed < fadeDuration) {
            game.getStats()->setAlpha(globalAlpha);
            TetrisPart->setAlpha(globalAlpha);
            RhythmPart->setAlpha(globalAlpha);
        }
        return;
    }

    if (videoPlayer != nullptr) {
        if (videoPlayer->isPlaying()) videoPlayer->update();
    }
    TetrisPart->update(game, renderer);
    RhythmPart->update(game, renderer);
	game.getStats()->Update(0, NONE, renderer);
}

void IngameState::render(Game& game, SDL_Renderer* renderer)  {
    TetrisPart->setAlpha(globalAlpha);
    RhythmPart->setAlpha(globalAlpha);
    game.getStats()->setAlpha(globalAlpha);

    if (videoPlayer != nullptr) {
        videoPlayer->render();
    }
	drawBlackLowOpacityRect(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, globalAlpha * 0.4f);
    TetrisPart->render(game, renderer);
    RhythmPart->render(game, renderer);
    game.getStats()->Render(renderer);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
}
