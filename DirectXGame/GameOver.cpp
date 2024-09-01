#include "GameOver.h"
#include <DebugText.h>
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>

GameOver::~GameOver() {
	delete gameOverModel_;
	delete wall2Model_;
}

void GameOver::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	for (int i = 0; i < 2; i++) {
		viewProjection_[i].Initialize();
		worldTransform_[i].Initialize();
	}
	gameOverModel_ = Model::CreateFromOBJ("GameOver", true);
	worldTransform_[0].translation_.z = -43;

	wall2Model_ = Model::CreateFromOBJ("wall2", true);
	worldTransform_[1].scale_ = Vector3(10.0f, 10.0f, 1.0f);
}

void GameOver::Update() {
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		restart_ = true;
	}

	for (int i = 0; i < 2; i++) {
		worldTransform_[i].UpdateMatrix();
	}
}

void GameOver::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	gameOverModel_->Draw(worldTransform_[0], viewProjection_[0]);
	wall2Model_->Draw(worldTransform_[1], viewProjection_[1]);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
