#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class Texture {
public:
	Texture();
	~Texture();

	void loadFromFile(SDL_Renderer* renderer, std::string& filepath);
	void setAlpha(float a);
	void FullRender(SDL_Renderer* renderer, int x, int y);
	void PartialRender(SDL_Renderer* renderer, SDL_FRect* src, SDL_FRect* dest);
private:
	SDL_Texture* texture;
};

class Text {
public:
	Text();
	~Text();

	void loadFont(std::string path, int FontSize);
	void setAlpha(float a);
	void loadTextToTexture(std::string text, SDL_Color color, SDL_Renderer* renderer);
	void getSize(int* x, int* y);
	void Render(SDL_Renderer* renderer, int x, int y);
	SDL_Texture* getTexture() { return texture; };
private:
	SDL_Texture* texture;
	SDL_Color textColor;
	TTF_Font* font;
	std::string text;
};
#endif