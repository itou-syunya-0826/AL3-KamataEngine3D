#include "Enemy.h"
#include <cassert>
#include <numbers>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = (Vector3)position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 5.f / 2.f;
	viewProjection_ = viewProjection;
}

void Enemy::Update() { 
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { 
	model_->Draw(worldTransform_, *(viewProjection_), textureHandle_);
}
