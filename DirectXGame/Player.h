#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DataStore.h"

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:

	Player();
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model_">モデルのポインタ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection,const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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
	/// テクスチャハンドル
	/// </summary>
	uint32_t textureHandle_ = 0;
	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	ViewProjection* viewProjection_ = nullptr;
	/// <summary>
	/// 速度
	/// </summary>
	MyVector3 velocity_ = {};

	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;
	//接地状態フラグ
	bool onGround_ = true;

	
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation  = 0.01f;
	static inline const float kLimitRunSpeed = 0.3f;
	static inline const float kTimeTurn = 0.3f;

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.5f;
	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 1.;
	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 3.45f;



};


