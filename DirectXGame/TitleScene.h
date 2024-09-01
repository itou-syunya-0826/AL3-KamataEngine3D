#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:

	TitleScene() = default;
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	// デスフラグのgetter
	bool IsStart() const { return isStart_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ワールド変換データ
	/// </summary>
	WorldTransform worldTransform_[3];

	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	ViewProjection viewProjection_[3];
	/// <summary>
	/// モデル
	/// </summary>
	Model* titleModel_;
	Model* buttonModel_;
	Model* wallModel_;

	// 終了フラグ
	bool isStart_ = false;
};
