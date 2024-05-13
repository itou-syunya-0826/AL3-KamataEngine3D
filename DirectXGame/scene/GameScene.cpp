#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Matrix.h"
#include "ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete blockmodel_;
	delete player_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine){
			delete worldTransformBlock;
		}
	}
	
	worldTransformBlocks_.clear();

	delete debugCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	blockTextureHandle_ = TextureManager::Load("cube/cube.jpg");
	blockmodel_ = Model::Create();
	viewProjection_.Initialize();

	player_ = new Player();
	player_->Initialize(blockmodel_, blockTextureHandle_, &viewProjection_);

	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i + j) % 2 !=0) {
				continue;
			}
			worldTransformBlocks_[i][j]= new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}


	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

}

void GameScene::Update() { 
	player_->Update(); 

	/*for (const auto& vt : worldTransformBlocks_) {
		for (auto t : vt) {
			if (!)
		}
	}*/

	/*for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {

		worldTransformBlock->matWorld_ = Matrix::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

		worldTransformBlock->TransferMatrix();

	}*/

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			
			worldTransformBlock->matWorld_ = Matrix::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}

	//デバックカメラの更新
	debugCamera_->Update();

	#ifdef _DEBUG 
	//排他的論理和
	if (input_->TriggerKey(DIK_BACK)) {
		isDebugCameraActive_ ^= true;
	}

	/*if (input_->TriggerKey(DIK_BACK)) { 
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} 
		else {
			isDebugCameraActive_ = true;
		}
	}*/
	#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
		
}

void GameScene::Draw() {

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
	player_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			
			blockmodel_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
		}
	}

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
