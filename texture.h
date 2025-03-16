#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class TextureManager {
public:
	TextureManager();
	void Clear();
	bool loadFont(const std::string& path, int fontSize);
	bool loadTextToTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer);
	bool loadFromFile(std::string path, SDL_Renderer* renderer);
	void Render(int x, int y, SDL_Renderer* renderer, SDL_FRect* clip);
private:
	int x, y, w, h;
	SDL_Texture* texture;
	TTF_Font* cFont = NULL;
};

#endif