#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

#include "game.h"

class Button {
public:
	Button();
	~Button();

	void InitWithText(std::string text, SDL_Color color, SDL_Renderer* renderer, int x, int y);

	void handleEvent(Game& game, SDL_Event &event);
	void Update(Game& game, SDL_Renderer* renderer);
	void Render(Game& game, SDL_Renderer* renderer);

	const bool Clicked() { if (Contents) return isClicked; else return false; };
	void setAlpha(Uint8 a) { if (Contents) Contents->setAlpha(a); color.a = a; };
	std::string getText() { return text; };
private:
	std::string text; SDL_Color color;
	Text* Contents = NULL;
	bool isHovered = false;
	bool isClicked = false;
	int x, y, w, h;
};

#endif
