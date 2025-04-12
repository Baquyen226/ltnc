#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"
#include "constants.h"

class Background {
public:
	Background() {}
	~Background() {}
	void loadFromFile(SDL_Renderer* renderer, std::string& path) { texture->loadFromFile(renderer, path); }
	void setAlpha(Uint8 a) { alpha = a; texture->setAlpha(a); }
	void render(SDL_Renderer* renderer);
private:
	Texture* texture = NULL;
	SDL_FRect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Uint8 alpha = 255;
};

#endif