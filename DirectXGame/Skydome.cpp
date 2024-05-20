#include "Skydome.h"
#include <cassert>

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, ViewProjection* viewProjection,uint32_t texture) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	textureHandle_ = texture;
}

void Skydome::Update() { 
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_,textureHandle_); }
