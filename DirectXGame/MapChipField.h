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

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {

public:

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; };
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };
	//指定座標がマップチップの何番の位置にあるのかを計算する関数
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GetRectByindex(uint32_t xIndex, uint32_t yIndex);

private:

	// 1.ブロックサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	//　ブロックの個数
	static inline const int kNumBlockVirtical = 20; 
	static inline const int kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

};






