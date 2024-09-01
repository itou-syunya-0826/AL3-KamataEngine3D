#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"

class Player;

class Goal {
public:

	Goal() = default;
	~Goal() = default;

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	// ワールド座標を取得
	Vector3 GetWorldPotision();
	// ゴールのAABB（当たり判定）を取得
	AABB GetAABB();
	// 衝突応答
	void OnCollision(const Player* player);

private:
	/// <summary>
	/// ワールド変換データ
	/// </summary>
	WorldTransform worldTransform_;
	/// <summary>
	/// モデル
	/// </summary>
	Model* model_ = nullptr;
	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	ViewProjection* viewProjection_ = nullptr;

	// ゴールの当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 3.0f;
};
