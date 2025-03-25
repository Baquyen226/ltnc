#pragma once
#include <SDL.h>

#include "texture.h"
#include "constants.h"
class Background {
public:
    Background();
    Background(const std::string& imagePath, SDL_Renderer* renderer);

    ~Background();

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    void setVelocity(float v) { velocity = v; }
    void setPosition(float x, float y) { srcx = x; srcy = y; }
private:
	float velocity = 0, cycle = 0, srcx = 0, srcy = 0, srcw = SCREEN_WIDTH, srch = SCREEN_HEIGHT;
    float max_delta;
    SDL_FRect srcRect;
	std::string path = "";
    TextureManager* textureManager;
};
