#include "GameClear.h"
#include <DebugText.h>
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>

GameClear::~GameClear() { 
	delete gameClearModel_;
	delete wall3Model_;
}

void GameClear::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	for (int i = 0; i < 2; i++) {
		viewProjection_[i].Initialize();
		worldTransform_[i].Initialize();
	}

	gameClearModel_ = Model::CreateFromOBJ("GameClear", true);
	worldTransform_[0].translation_ = Vector3(0, 0, -45.0f);
	worldTransform_[0].scale_ = Vector3(1.0f, 1.0f, 1.0f);

	wall3Model_ = Model::CreateFromOBJ("wall3", true);
	worldTransform_[1].translation_ = Vector3(0, 0, 20.0f);
	worldTransform_[1].scale_ = Vector3(20.0f, 20.0f, 1.0f);
}

void GameClear::Update() {

	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		returned_ = true;
	}

	for (int i = 0; i < 2; i++) {
		worldTransform_[i].UpdateMatrix();
	}
}

void GameClear::Draw() {
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
	gameClearModel_->Draw(worldTransform_[0], viewProjection_[0]);
	wall3Model_->Draw(worldTransform_[1], viewProjection_[1]);

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
