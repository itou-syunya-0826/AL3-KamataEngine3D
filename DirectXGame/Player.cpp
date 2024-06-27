#define NOMINMAX
#include "Player.h"
#include <DebugText.h>
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>

Player::Player() {}

Player::~Player() {}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
/// <param name="viewProjection"></param>
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = (Vector3)position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 5.f / 2.f;
	viewProjection_ = viewProjection;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {
	// ①移動入力

	if (onGround_) {
		// 移動入力
		// 左右移動入力
		if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_D)) {
				if (lrDirection_ != LRDirection::kRight) {
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
					lrDirection_ = LRDirection::kRight;
				}

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				if (lrDirection_ != LRDirection::kLeft) {
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
					lrDirection_ = LRDirection::kLeft;
				}

				if (velocity_.x < 0.f) {
					velocity_.x *= (1.f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}

			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1 - kAttenuation);
			if (velocity_.x * velocity_.x < 0.001f) {
				velocity_.x = 0;
			}
		}

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			velocity_.y += kJumpAcceleration;
		}

		// ⑦旋回処理
		if (turnTimer_ > 0.f) {
			turnTimer_ -= 1.f / 60;
			float destinationRotationYTable[] = {std::numbers::pi_v<float> * 5.f / 2.f, std::numbers::pi_v<float> * 3.f / 2.f};
			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			float TimeRatio = 1 - turnTimer_ / kTimeTurn;
			float easing = TimeRatio;
			float nowRotationY = std::lerp(turnFirstRotationY_, destinationRotationY, easing);
			worldTransform_.rotation_.y = nowRotationY;
		}
	} else {
		velocity_.y -= kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// ②移動量を加味して衝突判定する

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// マップ衝突をチェック
	CollisionMap(collisionMapInfo);

	velocity_ = collisionMapInfo.move;


	// 移動
	worldTransform_.translation_ += velocity_;
	bool landing = false;

	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 2) {
			landing = true;
		}
	}


	// 着地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み
			worldTransform_.translation_.y = 2.f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// ⑧行列計算
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() { model_->Draw(worldTransform_, *(viewProjection_), textureHandle_); }

void Player::CollisionMap(CollisionMapInfo& info) {
	// マップ衝突判定上方向
	CollisionMapTop(info);
	////マップ衝突判定下方向
	CollisionMapBottom(info);
	////マップ衝突判定右方向
	// CollisionMapRight(info);
	////マップ衝突判定左方向
	// CollisionMapLeft(info);
}

void Player::CollisionMapTop(CollisionMapInfo& info) {

	// 移動後の4つの角の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool TopHit = false;
	
	IndexSet indexSet;
	float distToIndex = 0;     // 垂直の距離
	float midHeightoffset = 0; // 高さの中心にオフセットを加える
	float moveY = 0;
	float Height = kheight / 2.0f;

	// 左上点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		TopHit = true;
	}
	// 右上点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		TopHit = true;
	}

	if (TopHit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + Height) + velocity_);
		// めり込み先ブロックの範囲矩形(?)
		Rect rect = mapChipField_->GetRectByindex(indexSet.xIndex, indexSet.yIndex);

		// 移動量を求める
	
		distToIndex = rect.bottom - worldTransform_.translation_.y;
		midHeightoffset = kheight / 2 + kBlank;

		moveY = distToIndex - midHeightoffset;

		info.move.y = std::max(0.0f, moveY);

		// 天井に当たったことを記録する
		info.ceiling = true;
	}
	Player::HandleCeilingCollision(info);
}

void Player::CollisionMapBottom(CollisionMapInfo& info) {

	// 移動後の4つの角の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 下降あり？
	if (info.move.y >= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool BottomHit = false;

	// 左下点の当たり判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		BottomHit = true;
	}
	// 右下点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		BottomHit = true;
	}

	if (BottomHit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		// めり込み先ブロックの範囲矩形(?)
		Rect rect = mapChipField_->GetRectByindex(indexSet.xIndex, indexSet.yIndex);

		// 移動量を求める
		float distToIndex = 0;     // 垂直の距離
		float midHeightoffset = 0; // 高さの中心にオフセットを加える
		distToIndex = rect.top - indexSet.yIndex;
		midHeightoffset = -kheight / 2 + kBlank;
		velocity_.y = 0;
		float moveY = distToIndex + midHeightoffset;

		info.move.y = std::min(0.0f, moveY);

		// 地面に当たったことを記録する
		info.landing = true;
		Player::HandleCeilingCollision(info);
	}
}

#pragma warning(push)
#pragma warning(disable : 4100)
void Player::CollisionMapRight(CollisionMapInfo& info) {}

void Player::CollisionMapLeft(CollisionMapInfo& info) {}

// ③判定結果を反映して移動させる
void Player::ProcessMovemennt(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_.y += info.move.y;
}

// ④天井に接触している場合の処理
void Player::HandleCeilingCollision(const CollisionMapInfo& info) {

	//// 天井に当たった？
	//if (info.ceiling || info.landing) {
	//	DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
	//	velocity_.y = 0;
	//}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[static_cast<uint32_t>(Corner::kNumCorner)] = {
	    {+kWidth / 2.0f, -kheight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kheight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kheight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kheight / 2.0f, 0}, //  kLeftTop
	};

	return offsetTable[static_cast<uint32_t>(corner)] + center;
}

// ⑤壁に接触している場合の処理

// ⑥接地状態の切り替え

const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

const Vector3& Player::GetVelocity() { return velocity_; }
