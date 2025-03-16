#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "texture.h"

TextureManager::TextureManager() {
	texture = NULL;
    cFont = NULL;
    x = 0;
    y = 0;
	w = 0;
	h = 0;
}


void TextureManager::Clear() {
	SDL_DestroyTexture(texture);
	texture = NULL;
}

//Handles texts

bool TextureManager::loadFont(const std::string& path, int fontSize) {
    std::cerr << "Attempting to open font at: " << path << std::endl;
    cFont = TTF_OpenFont(path.c_str(), fontSize);
    if (!cFont) {
        std::cerr << "Failed to load font! TTF_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cerr << "Font loaded successfully!" << std::endl;
        return true;
    }
    return true;
}

bool TextureManager::loadTextToTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer) {
    Clear();

    size_t len = text.length();
    SDL_Surface* textSurface = TTF_RenderText_Solid(cFont, text.c_str(), len, color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! TTF_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    w = textSurface->w;
    h = textSurface->h;

    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    if (!texture) {
        std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}


//Handles iamges
bool TextureManager::loadFromFile(std::string path, SDL_Renderer* renderer) {
    Clear(); // Free old texture

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) return false;

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    w = loadedSurface->w;
    h = loadedSurface->h;

    SDL_DestroySurface(loadedSurface);
    return texture != NULL;
}


void TextureManager::Render(int x, int y, SDL_Renderer* renderer, SDL_FRect* clip) {
    SDL_FRect dest = { x, y, w, h };
    if (clip) {
        dest.w = clip->w;
        dest.h = clip->h;
    }
    SDL_RenderTexture(renderer, texture, clip, &dest);
}