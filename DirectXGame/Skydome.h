#pragma once
#include <model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

class Skydome {
private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	uint32_t textureHandle_ = 0;

	

public:

	Skydome();
	~Skydome();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
};
