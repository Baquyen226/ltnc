#pragma once
#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <SDL.h>

#include "constants.h"
#include "texture.h"

class Text {
public:
	Text();
	Text(std ::string &content, int x, int y, float FontSize);
	void Render(SDL_Renderer* renderer);
private:
	std::string CONTENTS;
	int x, y;
	float fontSize;
	TextureManager textureManager;
};

class Button {
public:
	std::string name = "";
	Button();
	Button(const std::string &name, const int x, const int y, const int w, const int h, const std::string &fontPath, const std::string &text);
	Button(const std::string& _name, const std::string& sprite_path, SDL_Renderer* renderer);
	bool isClicked(int x, int y);
	void Render(SDL_Renderer* renderer);
	void Render(SDL_Renderer* renderer, SDL_FRect* src, SDL_FRect* dest);
private:
	int x = 0, y = 0, w = 0, h = 0;
	std::string text = "";
	TextureManager textureManager;
};


class CheckBox {
public:
	CheckBox();
	CheckBox(std::string& TITLE, int x, int y, int size, bool checked);
	bool isClicked(int x, int y);
	void Render(SDL_Renderer* renderer);
private:
	int x, y, size;
	std::string TITLE;
	bool checked;
	TextureManager textureManager;
};

class Slider {
public:
	Slider();
	Slider(std::string &t, int x, int y, float min, float max, float cur);
	void handleEvents(SDL_Event& e);
	void Render(SDL_Renderer* renderer);
	float getValue();
private:
	int x, y, w = 400, h = 15;
	std::string TITLE;
	float minValue, maxValue, currentValue;
	bool dragging;
	TextureManager textureManager;
};

#endif