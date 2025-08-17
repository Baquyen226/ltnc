#pragma once
#ifndef MAPDATALOADER_H
#define MAPDATALOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include <SDL.h>

#include "constants.h"
#include "HelperDataType.h"

class MapDataLoader {
public:
	MapDataLoader();
	~MapDataLoader();
	bool loadMapData();
	const std::vector<MapData>& getMapDataList() { return mapDataList; }
private:
	std::string mapFolderPath = MAP_FOLDER_PATH;
	std::vector<MapData> mapDataList;
};
#endif
