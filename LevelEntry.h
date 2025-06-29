#pragma once
#ifndef LEVELENTRY_H
#define LEVELENTRY_H

#include <SDL.h>

#include "game.h"
#include "HelperDataType.h"
#include "constants.h"

class LevelEntry {
public:
	//this creates an entry from an existing mapdata
	LevelEntry(MapData mapdata, Game& game);
	~LevelEntry();
	void setAlpha(Uint8 a) { alpha = a; }
	void setPosition(int x, int y) { position.x = x; position.y = y; }
	void setSize(int w, int h) { position.w = w; position.h = h; }
	void setTexture(SDL_Texture* t) { texture = t; }
	SDL_Texture* getTexture() { return texture; };
	void Render(Game& game, SDL_Renderer* renderer, int x, int y);
	void Render(Game& game, SDL_Renderer* renderer, SDL_FRect* srcrect, SDL_FRect* destrect);
	bool isClicked(int x, int y);
private:
	SDL_Texture* texture = nullptr;
	SDL_Rect position;
	Uint8 alpha = 255;
};
#endif