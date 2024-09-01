#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameOver {
public:
	GameOver() = default;
	~GameOver();

	void Initialize();
	void Update();
	void Draw();

	bool IsReStart() const { return restart_; }

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_[2];
	ViewProjection viewProjection_[2];
	
	Model* gameOverModel_;
	Model* wall2Model_;

	bool restart_ = false;
};
