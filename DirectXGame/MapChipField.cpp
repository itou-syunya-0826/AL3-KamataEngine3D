#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>
#include <math.h>

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

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
    
    //水平
    if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
    }
    //垂直
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

    return mapChipData_.data[yIndex][xIndex]; 


}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {

    return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);

}

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	int32_t XIndex = static_cast<uint32_t>(
        (position.x + kBlockWidth / 2) / kBlockWidth
    );
	int32_t YIndex = static_cast<uint32_t>(
        (position.y + kBlockHeight / 2) / kBlockHeight
    );

    int32_t maxIndexY = kNumBlockVirtical - 1;
	YIndex = maxIndexY - YIndex;

    IndexSet indexSet = {};

    indexSet.xIndex = XIndex;
    indexSet.yIndex = YIndex;
    
    return indexSet;

}

Rect MapChipField::GetRectByindex(uint32_t xIndex, uint32_t yIndex) {
    //指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
    
    Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

    return rect;
}


