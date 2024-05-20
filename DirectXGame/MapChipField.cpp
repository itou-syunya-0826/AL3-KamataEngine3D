#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

namespace {
    std::map<std::string, MapChipType> mapChipTable = {
        {"0", MapChipType::kBlank},
        {"1", MapChipType::kBlock},
    };
}

void MapChipField::ResetMapChipData() { 

    mapChipData_.data.clear(); 
    mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
    }

}

void MapChipField::LoadMapChipCsv(const std::string& filePath) { 
    ResetMapChipData();

    std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

    std::stringstream mapChipCsv;

    mapChipCsv << file.rdbuf();

    file.close();

    for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
        getline(mapChipCsv, line);

        std::istringstream line_stream(line);

        for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');
			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
        }
    }
}

MapChipType MapChipField::GetChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
    
    if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
    }

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

    return mapChipData_.data[yIndex][xIndex]; 


}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {

    return Vector3(kBlockWidith * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);

}

MapChipType MapChipField::GetNumBlockVirtical() { 
    return MapChipType(kNumBlockVirtical);
}

MapChipType MapChipField::GetNumBlockHorizontal() { 
    return MapChipType(kNumBlockHorizontal);
}