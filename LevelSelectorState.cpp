#include <thread>

#include "LevelSelectorState.h"
#include "game.h"
#include "LevelEntry.h"

bool LevelSelectorState::enter(Game& game, SDL_Renderer* renderer) {
	bool loaded = false;
	mapDataLoader = new MapDataLoader();
	mapDataLoader->loadMapData();
	mapDataList = mapDataLoader->getMapDataList();
	
	if (mapDataList.empty()) {
		std::cerr << "No map data found in the specified folder.\n";
		return false;
	}
	for (const auto& mapData : mapDataList) {
		LevelEntry* entry = new LevelEntry(mapData, game);
		levelEntries.push_back(entry);
		/*std::cerr << "Map name: " << mapData.mapName << "\n";
		std::cerr << "Artist: " << mapData.artist << "\n";
		std::cerr << "Audio file path: " << mapData.audioFilePath << "\n";
		std::cerr << "Background file path: " << mapData.backgroundFilePath << "\n";
		std::cerr << "Duration: " << mapData.duration << "\n";
		std::cerr << "HP Drain Multiplier: " << mapData.hp_drain_multiplier << "\n";
		std::cerr << "Rating: " << mapData.rating << "\n";*/
	}
	return true;
}

bool LevelSelectorState::exit() {
	levelEntries.clear();
	if (mapDataLoader) delete mapDataLoader;
	mapDataLoader = NULL;
	for (auto& entry : levelEntries) {
		delete entry;
	}
	levelEntries.clear();
	return true;
}

void LevelSelectorState::handleEvent(Game& game, SDL_Event& event) {
	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_ESCAPE) {
			game.switchState(new MenuState(), game.getRenderer());
		}
	}

	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;
			for (size_t i = 0; i < levelEntries.size(); ++i) {
				if (levelEntries[i]->isClicked(mouseX, mouseY)) {
					//std::cerr << "Is this thing even printing(in LevelSelectorState/handleEvent)?\n";
					//mapDataList[i].print();
					//data is still available here
					game.enterIngameStateWithMap(&mapDataList[i]);
					return;
				}
			}
		}
	}
}

void LevelSelectorState::update(Game& game, SDL_Renderer* renderer) {}

void LevelSelectorState::render(Game& game, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer); // use Clear instead of FillRect

	int spacing = 12; // space between entries
	int y = 60;

	for (size_t i = 0; i < levelEntries.size(); ++i) {
		levelEntries[i]->Render(game, renderer, 700, y);
		y += INDIVIDUAL_MAP_H + spacing; // next entry's y position
	}

	SDL_RenderPresent(renderer);
}
