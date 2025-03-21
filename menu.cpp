#include <SDL.h>
#include <iostream>
#include <string>

#include "menu.h"
#include "sfx.h"

int buttoncount = 3;
std::string buttonClicked = "asset/sound/buttonclicked.wav";
Menu::Menu() {
}

void Menu::Render(SDL_Renderer* renderer) {
	for (auto& button : buttons) {
		//std::cout << "Rendering " << button.name << "\n";
		button.Render(renderer);
	}
}

int Menu::handleClicks(int x, int y) {
	for (int i = 0; i < buttoncount; i++) {
		if (buttons[i].isClicked(x, y)) {
			std::cerr << "Button " << buttons[i].name << " clicked!\n";
			if (!sfx.playSound(buttonClicked, GAME_EVENT)) std::cerr << "Sound file not found: " << buttonClicked << "\n";
			return i;
		}
	}
	return -1;
}