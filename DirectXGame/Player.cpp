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
	CollisionMapRight(info);
	////マップ衝突判定左方向
	 CollisionMapLeft(info);

	ToggleGrounding(info);
	HandleWallCollision(info);

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
	IndexSet indexSet;
	bool BottomHit = false;
	float Height = kheight / 2.0f;
	float distToIndex = 0;     // 垂直の距離
	float midHeightoffset = 0; // 高さの中心にオフセットを加える
	float moveY = 0;

	// 左下点の当たり判定
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
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + Height) + velocity_);
		// めり込み先ブロックの範囲矩形(?)
		Rect rect = mapChipField_->GetRectByindex(indexSet.xIndex, indexSet.yIndex);

		// 移動量を求める

		distToIndex = rect.top - worldTransform_.translation_.y;
		midHeightoffset = kheight / 2 + kBlank;
		moveY = distToIndex + midHeightoffset;

		info.move.y = std::min(0.0f, moveY);

		// 地面に当たったことを記録する
		info.landing = true;
	}
	if (!BottomHit) {
		onGround_ = false;
	}
}

void Player::CollisionMapRight(CollisionMapInfo& info) {

	// 移動後の4つの角の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 右移動あり？
	if (info.move.x <= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	IndexSet indexSet;
	bool RightHit = false;
	float Width = kWidth / 2.0f;
	float distToIndex = 0;    // 垂直の距離
	float midWidthoffset = 0; // 高さの中心にオフセットを加える
	float moveX = 0;

	// 右上点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		RightHit = true;
	}
	// 右下点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		RightHit = true;
	}

	// ブロックにヒット？
	if (RightHit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + Width) + velocity_);
		// めり込み先ブロックの範囲矩形(?)
		Rect rect = mapChipField_->GetRectByindex(indexSet.xIndex, indexSet.yIndex);

		// 移動量を求める

		distToIndex = rect.left - worldTransform_.translation_.x;
		midWidthoffset = kWidth / 2 + kBlank;
		moveX = distToIndex - midWidthoffset;

		info.move.x = std::max(0.0f, moveX);

		// 壁に当たったことを判定結果に記録する
		info.hitWall = true;
	}
}

void Player::CollisionMapLeft(CollisionMapInfo& info) {
	// 移動後の4つの角の座標
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 左移動あり？
	if (info.move.x >= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	IndexSet indexSet;
	bool LeftHit = false;
	float Width = kWidth / 2.0f;
	float distToIndex = 0;    // 垂直の距離
	float midWidthoffset = 0; // 横幅の中心にオフセットを加える
	float moveX = 0;

	// 左上点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		LeftHit = true;
	}
	// 左下点の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		LeftHit = true;
	}

	// ブロックにヒット？
	if (LeftHit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + Width) + velocity_);
		// めり込み先ブロックの範囲矩形(?)
		Rect rect = mapChipField_->GetRectByindex(indexSet.xIndex, indexSet.yIndex);

		// 移動量を求める

		distToIndex = rect.right - worldTransform_.translation_.x;
		midWidthoffset = kWidth / 2 + kBlank;
		moveX = distToIndex + midWidthoffset;

		info.move.x = std::min(0.0f, moveX);

		// 壁に当たったことを判定結果に記録する
		info.hitWall = true;
	}
}

// ③判定結果を反映して移動させる
void Player::ProcessMovemennt(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_.y += info.move.y;
}

// ④天井に接触している場合の処理
void Player::HandleCeilingCollision(const CollisionMapInfo& info) {

	// 天井に当たった？
	if (info.ceiling || info.landing) {
	    DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
	    velocity_.y = 0;
	}
}

// ⑤壁に接触している場合の処理
void Player::HandleWallCollision(const CollisionMapInfo& info) {
	// 壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

// ⑥接地状態の切り替え
void Player::ToggleGrounding(const CollisionMapInfo& info) {

	// 自キャラが接地状態？
	if (onGround_) {
		// 接地状態の処理
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			// 移動後の4つの角の座標
			std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			IndexSet indexSet;
			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool Hit = false;
			// 左下点の当たり判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kSmallNumber, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				Hit = true;
			}
			// 右下点の当たり判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kSmallNumber, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				Hit = true;
			}

			// 落下開始
			if (!Hit) {
				onGround_ = false;
			}
		}
	} else {
		// 空中状態の処理
		if (info.landing) {
			// 接地状態に切り替える（落下を止める）
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
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

const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

const Vector3& Player::GetVelocity() { return velocity_; }
