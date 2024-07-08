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
	/// <summary>
	/// 度をラジアンに変換
	/// </summary>
	/// <param name="dogress"></param>
	/// <returns></returns>
	float Dogress(float dogress);

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

	// 速度
	Vector3 velocity_ = {0, 0, 0};

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.05f;
	// 最初の角度
	static inline const float kWalkMotionAngleStart = -45.0f;
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 90.0f;
	// アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 3.0f;

	//経過時間
	float walkTimer_ = 0.0f;
	



};
