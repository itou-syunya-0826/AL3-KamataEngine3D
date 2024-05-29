#define NOMINMAX
#include "Player.h"
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
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection,const Vector3& position) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = (MyVector3)position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 5.f / 2.f;
	viewProjection_ = viewProjection;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() { 
	worldTransform_.UpdateMatrix();

	if (onGround_) {
		// 移動入力
		// 左右移動入力
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			MyVector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (lrDirection_ != LRDirection::kRight) {
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
					lrDirection_ = LRDirection::kRight;
				}

				if (velocity_.mValue.x < 0.0f) {
					velocity_.mValue.x *= (1.0f - kAttenuation);
				}
				acceleration.mValue.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (lrDirection_ != LRDirection::kLeft) {
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
					lrDirection_ = LRDirection::kLeft;
				}

				if (velocity_.mValue.x < 0.0f) {
					velocity_.mValue.x *= (1.0f - kAttenuation);
				}
				acceleration.mValue.x -= kAcceleration;
			}

			velocity_ += acceleration;
			velocity_.mValue.x = std::clamp(velocity_.mValue.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.mValue.x *= (1 - kAttenuation);
			if (velocity_.mValue.x * velocity_.mValue.x < 0.001f) {
				velocity_.mValue.x = 0;
			}
		}

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			velocity_.mValue.y += kJumpAcceleration;
		}

		if (turnTimer_ > 0.0f) {
			turnTimer_ -= 1.f / 60;

			float destinationRotationYTable[] = {std::numbers::pi_v<float> * 5.f / 2.f, std::numbers::pi_v<float> * 3.f / 2.f};

			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			float TimeRatio = 1 - turnTimer_ / kTimeTurn;
			float easing = TimeRatio;
			float nowRotationY = std::lerp(turnFirstRotationY_, destinationRotationY, easing);
			worldTransform_.rotation_.y = nowRotationY;
		}
	}
	else {
		velocity_.mValue.y -= kGravityAcceleration;
		velocity_.mValue.y = std::max(velocity_.mValue.y, -kLimitFallSpeed);
	}

	// 移動
	worldTransform_.translation_ += velocity_;
	bool landing = false;

	if (velocity_.mValue.y < 0) {
		if (worldTransform_.translation_.mValue.y <= 2) {
			landing = true;
		}
	}

	//着地判定
	if (onGround_) {
		//ジャンプ開始
		if (velocity_.mValue.y > 0.0f) {
			//空中状態に移行
			onGround_ = false;
		}
	} else {
		//着地
		if (landing) {
			//めり込み
			worldTransform_.translation_.mValue.y = 1.f;
			//
			velocity_.mValue.x *= (1.0f - kAttenuation);//?
			//
			velocity_.mValue.y = 0.0f;
			//
			onGround_ = true;
		}
	}
	//行列計算
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() { 
	model_->Draw(worldTransform_, *(viewProjection_), textureHandle_);
}
