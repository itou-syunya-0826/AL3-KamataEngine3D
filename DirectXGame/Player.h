#pragma once
#include "DataStore.h"
#include "MapChipField.h"
#include "Model.h"
#include "WorldTransform.h"

class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};
//角
enum Corner {
	// 右下
	kRightBottom,
	// 左下
	kLeftBottom,
	// 右上
	kRightTop,
	// 左上
	kLeftTop,

	// 要素数
	kNumCorner

};

// マップチップ
struct CollisionMapInfo {
	// 天井衝突フラグ
	bool ceiling = false;
	// 着地フラグ
	bool landing = false;
	// 壁接触フラグ
	bool hitWall = false;
	// 移動量
	Vector3 move;
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

	const WorldTransform& GetWorldTransform();
	const MyVector3& GetVelocity();

private:

	//マップ衝突判定
	void CollisionMap(CollisionMapInfo& info);
	void CollisionMapTop(CollisionMapInfo& info);//上
	void CollisionMapBottom(CollisionMapInfo& info);//下
	void CollisionMapRight(CollisionMapInfo& info);//右
	void CollisionMapLeft(CollisionMapInfo& info);//左

	//判定結果を反映して移動させる
	void ProcessMovemennt(const CollisionMapInfo& info);
	//天井に接触している場合の処理
	void HandleCeilingCollision(const CollisionMapInfo& info);
	//4つの角の内、指定した角の座標を得る関数
	Vector3 CornerPosition(const Vector3& center, Corner corner);

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
	/// <summary>
	/// 速度
	/// </summary>
	MyVector3 velocity_ = {0, 0, 0};

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 接地状態フラグ
	bool onGround_ = true;

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 0.3f;
	static inline const float kTimeTurn = 0.3f;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.01f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.3f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.4f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kheight = 0.8f;

	//余白
	static inline const float kBlank = 0.1f;
};
