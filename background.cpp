#include "background.h"

Background::Background() {
	path = "";
	velocity = 0; cycle = 0.0000628f; srcx = 0; srcy = 0, max_delta = 130;
	textureManager = NULL;
};


Background::Background(const std::string& imagePath, SDL_Renderer* renderer) {
	path = imagePath;
	velocity = 0; cycle = 0.0000628f; srcx = 0; srcy = 0, max_delta = 130;
	textureManager = new TextureManager();
	textureManager->loadFromFile(path, renderer);
}

Background::~Background() {
	if (textureManager) textureManager = NULL;
}

void Background::Update(float timeElapsed) {
	srcx = max_delta + max_delta * sin(timeElapsed * cycle);
	srcy = 0;
}

void Background::Render(SDL_Renderer* renderer) {
	SDL_FRect srcDest = { srcx, srcy, srcw, srch };
	textureManager->Render(0, 0, renderer, &srcDest);
}