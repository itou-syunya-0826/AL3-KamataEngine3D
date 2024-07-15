#include "Deathparticles.h"

Deathparticles::Deathparticles() {}
Deathparticles::~Deathparticles() {}

void Deathparticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	model_ = model;
	viewProjection_ = viewProjection;
}

void Deathparticles::Update() {
	for (auto& worldTransforms : worldTransforms_) {
		worldTransforms.UpdateMatrix();
	}
}

void Deathparticles::Draw() {
	for (auto& worldTransforms : worldTransforms_) {
		model_->Draw(worldTransforms, *(viewProjection_));
	}
}
