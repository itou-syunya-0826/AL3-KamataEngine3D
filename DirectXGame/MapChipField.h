#pragma once
#include <vector>
#include <string>
#include "Vector3.h"


enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {

public:

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetNumBlockVirtical();
	MapChipType GetNumBlockHorizontal();


private:

	// 1.ブロックサイズ
	static inline const float kBlockWidith = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	//　ブロックの個数
	static inline const int kNumBlockVirtical = 20; 
	static inline const int kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

};






