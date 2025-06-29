#include "Slider.h"

Slider::Slider() {
	TITLE = "";
	x = 0;
	y = 0;
	minValue = 0;
	maxValue = 0;
	currentValue = 0;
	dragging = false;
};

Slider::Slider(const std::string& t, int _x, int _y, int _w, int _h, float _min, float _max, float _cur) {
	TITLE = t;
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	minValue = _min;
	maxValue = _max;
	currentValue = _cur;
	dragging = false;
	text = new Text();
	text->loadFont("assets/fonts/8-bit.ttf", 24);
}

void Slider::Render(SDL_Renderer* renderer) {
	SDL_FRect bar = { x, y, w, h };
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, alpha);
	SDL_RenderFillRect(renderer, &bar);
	int knobX = x + ((currentValue - minValue) * w) / (maxValue - minValue);
	SDL_FRect knob = { knobX - 5, y - h / 4 + 10, 10, h };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &knob);

	std::string s = std::to_string((int)currentValue);
	text->loadTextToTexture(TITLE, { 255, 255, 255, alpha }, renderer);
	text->Render(renderer, x - 120, y);

	text->loadTextToTexture(s, { 255, 255, 255, alpha }, renderer);
	text->Render(renderer, x + w + 60, y);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Slider::handleEvents(SDL_Event& e) {
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int mx = e.button.x;
		int my = e.button.y;
		if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
			dragging = true;
		}
	}
	else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
		dragging = false;
	}
	else if (e.type == SDL_EVENT_MOUSE_MOTION && dragging) {
		int mx = e.motion.x;
		currentValue = minValue + ((mx - x) * (maxValue - minValue)) / w;
		if (currentValue < minValue) currentValue = minValue;
		if (currentValue > maxValue) currentValue = maxValue;
	}
}

float Slider::getValue() {
	return currentValue;
}
