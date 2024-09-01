#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameClear {
public:

	GameClear() = default;
	~GameClear();

	void Initialize();
	void Update();
	void Draw();

	bool IsReturn() const { return returned_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_[2];
	ViewProjection viewProjection_[2];
	
	Model* gameClearModel_;
	Model* wall3Model_;

	// クリアからタイトルに戻るためのフラグ
	bool returned_ = false;

};
