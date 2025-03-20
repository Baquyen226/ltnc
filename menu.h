#pragma once
#ifndef MENU_H
#define MENU_H

#include <vector>

#include "UI.h"
#include "constants.h"

class Menu {
public:
	Menu();
	void Render(SDL_Renderer* renderer);
	int handleClicks(int x, int y);
private:
    std::vector<Button> buttons = {
    Button("start", BUTTON_CONFIG_X, BUTTON_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_Y,
           "asset/font/8-bit.ttf", "Start"),

    Button("config", BUTTON_QUIT_X, BUTTON_CONFIG_Y, BUTTON_SIZE_X, BUTTON_SIZE_Y,
           "asset/font/8-bit.ttf", "Config"),

    Button("exit", BUTTON_QUIT_X,   BUTTON_QUIT_Y, BUTTON_SIZE_X, BUTTON_SIZE_Y,
           "asset/font/8-bit.ttf", "Exit")
    };

};

#endif