#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "texture.h"
#include "constants.h"

class Slider {
public:
	Slider();
	Slider(const std::string& t, int x, int y, int w, int h, float min, float max, float cur);
	void handleEvents(SDL_Event& e);
	void Render(SDL_Renderer* renderer);
	float getValue();
	void setAlpha(float a) { alpha = a; };
private:
	int x, y, w, h;
	Uint8 alpha = 255.0f;
	std::string TITLE;
	float minValue, maxValue, currentValue;
	bool dragging;
	Text* text = nullptr;
};

#endif