#include "MapDataLoader.h"

//helper function
std::string trimSpaceLeft(std::string &str) {
	int n = str.find_first_not_of(' ');
	return str.substr(n);
}

//classes
MapData::MapData() : mapName(""), artist(""), audioFilePath(""), backgroundFilePath(""), duration(0), noteTimestamps({}), hp_drain_multiplier(0), rating(0), directoryName("") {}
void MapData::print() {
	std::cerr << "Map Name: " << mapName << "\n";
	std::cerr << "Artist: " << artist << "\n";
	std::cerr << "Audio File Path: " << audioFilePath << "\n";
	std::cerr << "Background File Path: " << backgroundFilePath << "\n";
	std::cerr << "Duration: " << duration << "\n";
	std::cerr << "HP Drain Multiplier: " << hp_drain_multiplier << "\n";
	std::cerr << "Rating: " << rating << "\n";
	std::cerr << "Note Timestamps: ";
	for (const auto& timestamp : noteTimestamps) {
		std::cerr << timestamp << " ";
	}
	std::cerr << "\n";
	std::cerr << "Directory Name: " << directoryName << "\n";
}

MapDataLoader::MapDataLoader() { mapFolderPath = MAP_FOLDER_PATH; }
MapDataLoader::~MapDataLoader() {}
bool MapDataLoader::loadMapData() {
	if (!std::filesystem::exists(mapFolderPath)) {
		std::cerr << "Directory not found: " << mapFolderPath << "\n";
		return false;
	}

	for (const auto& entry : std::filesystem::directory_iterator(mapFolderPath)) {
		if (entry.is_directory()) {
			MapData mapData;

			// open the mapdata file
			const std::string directoryName = entry.path().filename().string();
			std::cerr << "Folder name: " << directoryName << " found!\n";
			std::string mapDataContents = "assets/beatmap/" + directoryName + "/" + directoryName + ".bq"; // should be "bad apple/bad apple.bq"
			mapData.directoryName = "assets/beatmap/" + directoryName + "/";
			std::ifstream file(mapDataContents);
			if (!file.is_open()) {
				std::cerr << "Failed to open file: \"" << mapDataContents << "\" ,skipping this folder!\n";
				continue;
			}

			// parse the thing
			std::cerr << "Parsing file: " << mapDataContents << "\n";
			std::string line;
			while (std::getline(file, line)) {
				if (line.empty()) continue;
				std::stringstream ss(line);
				std::string key, value;
				if (line.find("[Song]") != std::string::npos) {
					while (std::getline(file, line) && line.find("[") == std::string::npos) {
						ss.clear();
						ss.str(line);
						std::getline(ss, key, '=');
						std::getline(ss, value);
						if (key == "name ") mapData.mapName = trimSpaceLeft(value);
						else if (key == "artist ") mapData.artist = trimSpaceLeft(value);
						else if (key == "audio ") mapData.audioFilePath = trimSpaceLeft(value);
						else if (key == "background ") mapData.backgroundFilePath = trimSpaceLeft(value);
						else if (key == "thumbnail ") mapData.thumbnailFilePath = trimSpaceLeft(value);
						else if (key == "duration ") mapData.duration = std::stoi(value);
					}
				}

				if (line.find("[Difficulty]") != std::string::npos) {
					while (std::getline(file, line) && line.find("[") == std::string::npos) {
						ss.clear();
						ss.str(line);
						std::getline(ss, key, '=');
						std::getline(ss, value);
						if (key == "hp_drain_multiplier ") mapData.hp_drain_multiplier = std::stoi(value);
						else if (key == "rating ") mapData.rating = std::stod(value);
					}
				}

				if (line.find("[Notes]") != std::string::npos) {
					while (std::getline(file, line) && line.find("[") == std::string::npos) {
						ss.clear();
						ss.str(line);
						int timestamp;
						while(ss >> timestamp) mapData.noteTimestamps.push_back(timestamp);
					}
				}
			}

			mapDataList.push_back(mapData);
			file.close();
			//mapData.print();
		}
	}
	return true;
}



