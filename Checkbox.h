#pragma once
#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "texture.h"
#include "game.h"

class Checkbox {
public:
	Checkbox();
	~Checkbox();

	void init(int x, int y, const std::string& label);
	void handleEvent(Game& game, SDL_Event& event);
	void render(SDL_Renderer* renderer);
	void setAlpha(float a);
	bool isEnabled() { return checked; };
private:
	void draw(bool checked, SDL_Renderer* renderer);

	std::string label;
	int x, y, w, h;
	float alpha = 255.0f;
	bool checked;
	SDL_Texture* texture = nullptr;
	TTF_Font* font = nullptr;
};

#endif