#include "Player.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}


/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
/// <param name="viewProjection"></param>
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() { 
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}
