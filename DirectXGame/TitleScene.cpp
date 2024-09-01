#include "TitleScene.h"
#include "imgui.h"

TitleScene::~TitleScene() {
	delete titleModel_;
	delete buttonModel_;
	delete wallModel_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	for (int i = 0; i < 3; i++) {
		viewProjection_[i].Initialize();
		worldTransform_[i].Initialize();
	}

	titleModel_ = Model::CreateFromOBJ("Title", true);
	worldTransform_[0].scale_ = Vector3(10.0f, 10.0f, 1.0f);
	worldTransform_[0].translation_.x = 0;
	worldTransform_[0].translation_.y = -5;

	buttonModel_ = Model::CreateFromOBJ("button", true);
	worldTransform_[1].translation_.y = -0.5;
	worldTransform_[1].translation_.z = -41;

	wallModel_ = Model::CreateFromOBJ("wall", true);
	worldTransform_[2].translation_ = Vector3(0, 0, 5.0f);
	worldTransform_[2].scale_ = Vector3(20.0f, 10.0f, 1.0f);
	
}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		isStart_ = true;
	}

	for (int i = 0; i < 3; i++) {
		worldTransform_[i].UpdateMatrix();
	}

}

void TitleScene::Draw() {

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
	titleModel_->Draw(worldTransform_[0], viewProjection_[0]);
	buttonModel_->Draw(worldTransform_[1], viewProjection_[1]);
	wallModel_->Draw(worldTransform_[2], viewProjection_[2]);

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
