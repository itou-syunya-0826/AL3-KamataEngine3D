#include "Enemy.h"
#include <cassert>
#include <numbers>
#define _USE_MATH_DEFINES
#include <math.h>

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

	//速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() { 

	//移動
	worldTransform_.translation_ += velocity_;

	//タイマー加算
	walkTimer_ += 1.f / 60.f;

	//回転アニメーション
	float param = std::sin((float)((2 * M_PI) * walkTimer_ / kWalkMotionTime));
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	float worldRotateX = 0;
	worldRotateX = Dogress(radian);
	worldTransform_.rotation_.x = worldRotateX;

	//行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { 
	model_->Draw(worldTransform_, *(viewProjection_), textureHandle_); 
}

float Enemy::Dogress(float dogress) { 
	return float(dogress * M_PI / 180);
}
