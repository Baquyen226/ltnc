#include <SDL.h>
#include <iostream>

#include "menu.h"

int buttoncount = 3;
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
			return i;
		}
	}
	return -1;
}