#pragma once
#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <SDL.h>

#include "constants.h"
#include "texture.h"

class Button {
public:
	std::string name = "";
	Button();
	Button(const std::string &name, const int x, const int y, const int w, const int h, const std::string &fontPath, const std::string &text);
	bool isClicked(int x, int y);
	void Render(SDL_Renderer* renderer);
private:
	int x, y, w, h;
	std::string text;
	TextureManager textureManager;
};

#endif