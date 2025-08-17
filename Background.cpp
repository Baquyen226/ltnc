#include "Background.h"

void Background::render(SDL_Renderer* renderer) {
	texture->PartialRender(renderer, 0, &backgroundRect);
	//overlay alpha
	int lAlpha = 96 * alpha / 255;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, lAlpha);
	SDL_RenderFillRect(renderer, &backgroundRect);
}