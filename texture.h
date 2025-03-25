#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

///Renders text or images
///TextureManager() - Constructor
///Clear() - Clears the texture
///loadFont() - Loads a font
///loadTextToTexture() - Loads text to texture(MUST LOAD FONT FIRST)
///loadFromFile() - Loads image from file
///Render() - Renders the texture
class TextureManager {
public:
	TextureManager();
	void Clear();
	bool loadFont(const std::string& path, int fontSize);
	bool loadTextToTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer);
	bool loadFromFile(std::string path, SDL_Renderer* renderer);
	void Render(int x, int y, SDL_Renderer* renderer, SDL_FRect* clip);
	void PartialRender(SDL_Renderer* renderer, SDL_FRect* src, SDL_FRect* clip);

	//literally no point in using class sgiosfkjhgodfijh
	int get_posX() { return x; }
	int get_posY() { return y; }
	int get_width() { return w; }
	int get_height() { return h; }

private:
	int x, y, w, h;
	SDL_Texture* texture;
	TTF_Font* cFont = NULL;
};

#endif