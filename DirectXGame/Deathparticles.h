#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <array>
#include <cmath>
#define M_PI 3.14159265358979323846
#include "Matrix.h"
#include <ObjectColor.h>
#include <algorithm>

/// <summary>
/// デス演出用パーティクル
/// </summary>
class Deathparticles {
public:

	Deathparticles();
	~Deathparticles();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model_">モデルのポインタ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	bool Isfinished() const { return isFinished_; }

private:

	/// <summary>
	/// モデル
	/// </summary>
	Model* model_ = nullptr;
	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	ViewProjection* viewProjection_ = nullptr;
	

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	// 存続時間（消滅までの時間）<秒>
	static inline const float kDuration = 2.0f;
	// 移動の速さ
	static inline const float kSpeed = 0.10f;
	// 分割した一個分の角度
	static inline const float kAngleUnit = (float)(2.0f * M_PI / 8.0f);

	//　終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_=0.0f;

	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;
	// パーティクルのアルファ値
	float alpha_ = 1.0f;

	// パーティクルのフラグ
	bool isParticle_ = true;

};
