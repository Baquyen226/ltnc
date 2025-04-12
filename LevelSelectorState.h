#pragma once
#ifndef LEVELSELECTORSTATE_H
#define LEVELSELECTORSTATE_H

#include <vector>
#include <SDL.h>

class LevelEntry;

#include "Button.h"
#include "GameState.h"
#include "MapDataLoader.h"
#include "LevelEntry.h"

class LevelSelectorState : public GameState {
public:
	bool enter(Game& game, SDL_Renderer* renderer) override;
	bool exit() override;
	void handleEvent(Game& game, SDL_Event& event) override;
	void update(Game& game, SDL_Renderer* renderer) override;
	void render(Game& game, SDL_Renderer* renderer) override;
	void setAlpha(Uint8 a);
private:
	std::vector<Button*> buttons;
	SDL_Color buttonColor = { 255, 255, 255, 255 };
	SDL_Color buttonHoverColor = { 96, 0, 0, 255 };
	SDL_Color buttonClickedColor = { 0, 0, 0, 255 };
	int buttonWidth = 200;
	int buttonHeight = 50;
	int buttonSpacing = 10;
	Texture background;
	SDL_Rect backgroundRect;
	float alpha = 255.0f;
	MapDataLoader* mapDataLoader = NULL;
	std::vector<MapData> mapDataList;
	std::vector<LevelEntry*> levelEntries;
	int selectedLevel = -1;
};
#endif