#include "Player.h"
#include <cassert>
#include <numbers>
#include <Input.h>


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
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() { 
	worldTransform_.UpdateMatrix();

	//移動入力
	//左右移動入力
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		MyVector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.mValue.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			acceleration.mValue.x -= kAcceleration;
		}
		velocity_ += acceleration;
	} else {
		velocity_.mValue.x *= (1 - kAttenuation);
		if (velocity_.mValue.x * velocity_.mValue.x < 0.001f) {
			velocity_.mValue.x = 0;
		}
	}


	//移動
	worldTransform_.translation_ += velocity_;

	//行列計算
	worldTransform_.UpdateMatrix();

}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() { 
	model_->Draw(worldTransform_, *(viewProjection_), textureHandle_);
}
