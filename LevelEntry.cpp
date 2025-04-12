#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "LevelEntry.h"
#include "constants.h"

LevelEntry::LevelEntry(MapData mapdata, Game& game) {
	SDL_Surface* surface = SDL_CreateSurface(INDIVIDUAL_MAP_W, INDIVIDUAL_MAP_H, SDL_PIXELFORMAT_ARGB8888);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_SetSurfaceAlphaMod(surface, alpha);

	TTF_Font* font = TTF_OpenFont("assets/fonts/8-bit.ttf", 24);
	if (font == NULL) {
		std::cerr << "Failed to load font: " << SDL_GetError() << "\n";
		return;
	}

	// Note that the entire level entry box is INDIVIDUAL_MAP_WxINDIVIDUAL_MAP_Hpx
	SDL_Color color = { 255, 255, 255, alpha };
	// Map name(Row 1)
	SDL_Surface* levelNameSurface = TTF_RenderText_Solid(font, mapdata.mapName.c_str(), mapdata.mapName.length(), color);
	if (levelNameSurface == NULL) {
		std::cerr << "Failed to level name surface: " << SDL_GetError() << "\n";
		return;
	}
	SDL_Rect levelNameRect = { THUMBNAIL_SIZE_X + 12 /*100(thumbnail) + 12(half font)*/, 12, levelNameSurface->w, 24};
	SDL_BlitSurface(levelNameSurface, NULL, surface, &levelNameRect);
	SDL_DestroySurface(levelNameSurface);

	// artist name(Row 2)
	SDL_Surface* artistSurface = TTF_RenderText_Solid(font, mapdata.artist.c_str(), mapdata.artist.length(), color);
	if (artistSurface == NULL) {
		std::cerr << "Failed to artist name surface: " << SDL_GetError() << "\n";
		return;
	}
	SDL_Rect artistNameRect = { THUMBNAIL_SIZE_X + 12 /*100(thumbnail) + 12(half font)*/, 36, artistSurface->w, 24 };
	SDL_BlitSurface(artistSurface, NULL, surface, &artistNameRect);
	SDL_DestroySurface(artistSurface);

	// duration(Row 3)
	std::string durationText = std::to_string(mapdata.duration / 1000) + "s";
	SDL_Surface* durationSurface = TTF_RenderText_Solid(font, durationText.c_str(), durationText.length(), color);
	if (durationSurface == NULL) {
		std::cerr << "Failed to duration surface: " << SDL_GetError() << "\n";
		return;
	}
	SDL_Rect durationRect = { THUMBNAIL_SIZE_X + 12 /*100(thumbnail) + 12(half font)*/, 60, durationSurface->w, 24 };
	SDL_BlitSurface(durationSurface, NULL, surface, &durationRect);
	SDL_DestroySurface(durationSurface);

	// rating (at the end of the box)
	std::string ratingText = "Difficulty: " + std::to_string(mapdata.rating);
	SDL_Surface* ratingSurface = TTF_RenderText_Solid(font, ratingText.c_str(), ratingText.length(), color);
	if (ratingSurface == NULL) {
		std::cerr << "Failed to rating surface: " << SDL_GetError() << "\n";
		return;
	}
	SDL_Rect ratingRect = { THUMBNAIL_SIZE_X + 12, INDIVIDUAL_MAP_H - 36, ratingSurface->w, 24 };
	SDL_BlitSurface(ratingSurface, NULL, surface, &ratingRect);
	SDL_DestroySurface(ratingSurface);


	//thumbnail
	std::string filePath;
	filePath = mapdata.directoryName + mapdata.thumbnailFilePath;
	SDL_Surface* thumbnail = IMG_Load(filePath.c_str());
	if (thumbnail == NULL) {
		std::cerr << "Failed to load thumbnail: " << SDL_GetError() << "\n" << "Rolling back to default thumbnail\n";
		filePath = "assets/images/nothumbnail.jpg";
		thumbnail = IMG_Load(filePath.c_str());
	}

	if (thumbnail == NULL) {
		std::cerr << "Failed to load thumbnail: " << SDL_GetError << "\n";
		return;
	}
	SDL_Rect thumbnailRect = { 0, 0, THUMBNAIL_SIZE_X, THUMBNAIL_SIZE_Y };
	SDL_BlitSurface(thumbnail, NULL, surface, &thumbnailRect);
	SDL_DestroySurface(thumbnail);

	
	texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
};

LevelEntry::~LevelEntry() {
	SDL_DestroyTexture(texture);
	texture = NULL;
}

void LevelEntry::Render(Game& game, SDL_Renderer* renderer, int x, int y) {
	if (texture) {
		SDL_FRect dstRect = { x, y, INDIVIDUAL_MAP_W, INDIVIDUAL_MAP_H }; // size of the entry
		setPosition(x, y);
		SDL_RenderTexture(renderer, texture, NULL, &dstRect);
	}
}

void LevelEntry::Render(Game& game, SDL_Renderer* renderer, SDL_FRect* rect, SDL_FRect* destrect) {
	if (texture) {
		SDL_RenderTexture(renderer, texture, rect, destrect);
	}
}

bool LevelEntry::isClicked(int x, int y) {
	return (x >= position.x && x <= position.x + INDIVIDUAL_MAP_W &&
		y >= position.y && y <= position.y + INDIVIDUAL_MAP_H);
}