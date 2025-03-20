#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class TextureManager {
	///<summary>
	///Renders text or images
	/// </summary>
	/// <remarks>
	///\n TextureManager() - Constructor
	///\n Clear() - Clears the texture
	///\n loadFont() - Loads a font
	///\n loadTextToTexture() - Loads text to texture(MUST LOAD FONT FIRST)
	///\n loadFromFile() - Loads image from file
	///\n Render() - Renders the texture
	/// </remarks>
	//didnt work >:-(
public:
	TextureManager();
	void Clear();
	bool loadFont(const std::string& path, int fontSize);
	bool loadTextToTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer);
	bool loadFromFile(std::string path, SDL_Renderer* renderer);
	void Render(int x, int y, SDL_Renderer* renderer, SDL_FRect* clip);

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