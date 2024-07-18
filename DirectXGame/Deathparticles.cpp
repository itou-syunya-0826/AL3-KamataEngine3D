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

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void Deathparticles::Update() {

	for (auto& worldTransforms : worldTransforms_) {
		worldTransforms.UpdateMatrix();
	}

	for (uint32_t i = 0; i < 8; ++i) {
		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		// 回転角の計算をする
		float angle = kAngleUnit * i;
		// Z軸周りの回転行列
		Matrix4x4 matrixRotation = Matrix::MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Matrix::Transform(velocity, matrixRotation);
		// 移動処理
		worldTransforms_[i].translation_ += velocity;
	}

	// カウンターを1フレーム文の秒数進める
	counter_ += 1.0f / 60.0f;
	// 存続時間の条件に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	alpha_ = 1.0f - (counter_ / kDuration);

	if (alpha_ <= 0.01f) {
		isParticle_ = false;
	}

	// 左の値を決める
	color_.w = std::clamp(alpha_, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();
}

void Deathparticles::Draw() {

	for (auto& worldTransforms : worldTransforms_) {
		if (isParticle_ == true) {
			model_->Draw(worldTransforms, *(viewProjection_), &objectColor_);
		}
	}
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}
}
