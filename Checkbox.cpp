#include <cmath>

#include "Checkbox.h"
#include "constants.h"

Checkbox::Checkbox() : x(0), y(0), checked(false), alpha(255), label("") {
	font = TTF_OpenFont("assets/fonts/8-bit.ttf", 24);
};
Checkbox::~Checkbox() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}


void Checkbox::init(int x, int y, const std::string& label) {
	this->x = x;
	this->y = y;
	this->label = label;
	font = TTF_OpenFont("assets/fonts/8-bit.ttf", 24);
}

void Checkbox::draw(bool checked, SDL_Renderer* renderer) {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	SDL_Color color = { 255, 255, 255, alpha };
	SDL_Surface* l = TTF_RenderText_Blended(font, label.c_str(), label.size(), color);

	w = l->w + CHECKBOX_WIDTH + CHECKBOX_TO_TEXT_OFFSET;
	h = std::max(l->h, CHECKBOX_HEIGHT);
	SDL_Surface* surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888);

	SDL_Rect checkboxRect = { CHECKBOX_WIDTH + CHECKBOX_TO_TEXT_OFFSET, 0, w, h + l->h / 2 };
	SDL_BlitSurface(l, nullptr, surface, &checkboxRect);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Checkbox::handleEvent(Game& game, SDL_Event& e) {
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
		float mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		if (mouseX >= x && mouseX <= x + CHECKBOX_WIDTH &&
			mouseY >= y && mouseY <= y + CHECKBOX_HEIGHT) {
			//std::cerr << label << " triggered!\n";
			checked = !checked; // Toggle checkbox state
			draw(checked, game.getRenderer());
		}
	}
}

void Checkbox::setAlpha(float a) {
	alpha = a;
	if (texture) {
		SDL_SetTextureAlphaMod(texture, alpha);
	}
}

void Checkbox::render(SDL_Renderer* renderer) {
	if (!texture) draw(checked, renderer);
	SDL_FRect r = { x, y, w, h };
	SDL_RenderTexture(renderer, texture, 0, &r);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
	r.w = CHECKBOX_WIDTH;
	r.h = CHECKBOX_HEIGHT;
	SDL_RenderRect(renderer, &r);

	if (checked) {
		int x1 = x;
		int y1 = y;
		int x2 = x + CHECKBOX_WIDTH;
		int y2 = y + CHECKBOX_HEIGHT;
		SDL_RenderLine(renderer, x1, y1, x2, y2);

		x1 += CHECKBOX_WIDTH;
		x2 -= CHECKBOX_WIDTH;
		SDL_RenderLine(renderer, x1, y1, x2, y2);
	}
}
