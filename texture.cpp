#include "texture.h"


//texture class
Texture::Texture() {
	texture = NULL;
};

Texture::~Texture() {
	if (texture) SDL_DestroyTexture(texture);
}


void Texture::loadFromFile(SDL_Renderer* renderer, std::string& filePath) {
	if (texture) SDL_DestroyTexture(texture);

	SDL_Surface* surface = IMG_Load(filePath.c_str());
	if (!surface) {
		std::cerr << "Surface is not loaded: " << SDL_GetError() << "\n";
		return;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "Texture is not loaded: " << SDL_GetError() << "\n";
		return;
	}
	SDL_DestroySurface(surface);
};

void Texture::setAlpha(float a) {
	if (!texture) {
		//std::cerr << "No texture for texture\n";
		return;
	}
	SDL_SetTextureAlphaMod(texture, a);
}


void Texture::FullRender(SDL_Renderer* renderer, int x, int y) {
	int w = texture->w;
	int h = texture->h;
	SDL_FRect dest = { x, y, w, h };
	SDL_RenderTexture(renderer, texture, 0, &dest);
}

void Texture::PartialRender(SDL_Renderer* renderer, SDL_FRect* src, SDL_FRect* dest) {
	SDL_RenderTexture(renderer, texture, src, dest);
}

//text class
Text::Text() {
	textColor = {};
	font = NULL;
	texture = NULL;
	text = "";
}

Text::~Text() {
	TTF_CloseFont(font);
	font = NULL;
	SDL_DestroyTexture(texture);
	texture = NULL;
}

void Text::loadFont(std::string path, int size) {
	font = TTF_OpenFont(path.c_str(), size);
	if (!font) {
		std::cerr << "Failed to load font: " << SDL_GetError() << "\n";
	}
}

void Text::loadTextToTexture(std::string text, SDL_Color color, SDL_Renderer* renderer) {
	textColor = color;
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), textColor);
	if (!surface) std::cerr << "An error occured why trying to load text to surface: " << SDL_GetError() << "\n";

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) std::cerr << "An error occured why trying to load surface to texture " << SDL_GetError() << "\n";

	SDL_DestroySurface(surface);
}

void Text::setAlpha(float a) {
	if (!texture) {
		std::cerr << "No texture for text\n";
		return;
	}
	SDL_SetTextureAlphaMod(texture, a);
}

void Text::getSize(int* w, int* h) {
	*w = texture->w;
	*h = texture->h;
}

void Text::Render(SDL_Renderer* renderer, int x, int y) {
	if (!texture) {
		std::cerr << "Texture was not found\n";
		return;
	}
	int w = texture->w;
	int h = texture->h;
	SDL_FRect rect = { x, y, w, h };
	SDL_RenderTexture(renderer, texture, 0, &rect);
}