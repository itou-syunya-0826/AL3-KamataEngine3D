#pragma once
#include "MapChipField.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="worldtransform"></param>
	/// <param name="viewprojection"></param>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:

	/// <summary>
	/// マップチップによるフィールド
	/// </summary>
	MapChipField* mapChipField_ = nullptr;
	/// <summary>
	/// ワールド変換データ
	/// </summary>
	WorldTransform worldTransform_;
	/// <summary>
	/// モデル
	/// </summary>
	Model* model_ = nullptr;
	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	uint32_t textureHandle_ = 0;
	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	ViewProjection* viewProjection_ = nullptr;


};
