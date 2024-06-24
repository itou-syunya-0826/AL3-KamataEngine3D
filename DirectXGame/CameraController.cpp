#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include <Input.h>
#include <algorithm>

CameraController::CameraController() {}

CameraController::~CameraController() {}

void CameraController::Initialize(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

void CameraController::Update() {

	// 追従対象のワールドトランスフォームを対象
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	targetCoordinates_.x = targetWorldTransform.translation_.x + targetOffset_.x + target_->GetVelocity().x * kVelocityBias; // うまく速度加算できてるかわからない
	targetCoordinates_.y = targetWorldTransform.translation_.y + targetOffset_.y + target_->GetVelocity().y * kVelocityBias;
	targetCoordinates_.z = targetWorldTransform.translation_.z + targetOffset_.z + target_->GetVelocity().z * kVelocityBias;

	// 座標補間よりゆったり追従
	viewProjection_->translation_.x = std::lerp(viewProjection_->translation_.x, targetCoordinates_.x, kInterpolationRate);
	viewProjection_->translation_.y = std::lerp(viewProjection_->translation_.y, targetCoordinates_.y, kInterpolationRate);
	viewProjection_->translation_.z = std::lerp(viewProjection_->translation_.z, targetCoordinates_.z, kInterpolationRate);

	// 移動範囲制限
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, targetWorldTransform.translation_.x + kMargin.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, targetWorldTransform.translation_.x + kMargin.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, targetWorldTransform.translation_.y + kMargin.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, targetWorldTransform.translation_.y + kMargin.top);

	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, targetWorldTransform.translation_.x + kMargin.left, targetWorldTransform.translation_.x + kMargin.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, targetWorldTransform.translation_.y + kMargin.bottom, targetWorldTransform.translation_.y + kMargin.top);

	// 行列を更新する
	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {

	// 追従対象のワールドトランスフォームを対象
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}
